/**********************************************************************
 *  mainwindow.cpp
 **********************************************************************
 *              Copyright (C) 2025 FormatUSB Team
 *
 *             Authors: FormatUSB Development Team
 *          Enhanced cross-platform USB formatting tool
 *            Modern GUI with improved USB detection
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#include "mainwindow.h"
#include "about.h"
#include "ui_mainwindow.h"
#include "version.h"

#include <QApplication>
#include <QCoreApplication>
#include <QFileDialog>
#include <QScrollBar>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QTextCursor>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>
#include <QProcess>
#include <QFileInfo>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <unistd.h>

MainWindow::MainWindow()
    : ui(new Ui::MainWindow)
{
    qDebug().noquote() << QCoreApplication::applicationName() << "version:" << VERSION;
    ui->setupUi(this);
    setWindowFlags(Qt::Window); // for the close, min and max buttons
    setup();
    ui->comboBoxUsbList->addItems(buildUsbList());
    this->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeUsb(const QString &options)
{
    // get a device value if action is on a partition
    QString device_to_check = device;
    if (device.contains("nvme")) {
        device_to_check = device.section("p", 0, 0);
    }
    if (device.contains("mmc")) {
        device_to_check = device.section("p", 0, 0);
    }
    if (device.contains("sd")) {
        device_to_check = device.left(3);
    }
    
    QString cmdstr = options;
    setConnections();
    qDebug() << "Executing format command:" << cmdstr;
    
    if (cmd) {
        cmd->start("/bin/bash", QStringList() << "-c" << cmdstr);
    }
}

// setup various items first time program runs
void MainWindow::setup()
{
    cmd = new Cmd(this);
    cmdprog = new Cmd(this);
    connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::cleanup);
    this->setWindowTitle("USB FORMAT v" + QString(VERSION));
    ui->buttonBack->setHidden(true);
    ui->stackedWidget->setCurrentIndex(0);
    ui->buttonCancel->setEnabled(true);
    ui->buttonNext->setEnabled(true);
    ui->outputBox->setCursorWidth(0);
    height = this->heightMM();
    ui->lineEditFSlabel->setText("USB-DATA");
    
    // Modern compact styling
    setStyleSheet(
        "QDialog { "
        "   background-color: #f8f9fa; "
        "   font-family: 'Segoe UI', Arial, sans-serif; "
        "}"
        "QPushButton { "
        "   background-color: #007bff; "
        "   color: white; "
        "   border: none; "
        "   padding: 8px 16px; "
        "   border-radius: 6px; "
        "   font-weight: 600; "
        "   min-width: 80px; "
        "} "
        "QPushButton:hover { background-color: #0056b3; } "
        "QPushButton:pressed { background-color: #004085; } "
        "QPushButton:disabled { background-color: #6c757d; } "
        "QComboBox { "
        "   border: 2px solid #dee2e6; "
        "   border-radius: 6px; "
        "   padding: 8px 12px; "
        "   background-color: white; "
        "   min-height: 20px; "
        "} "
        "QComboBox:focus { border-color: #007bff; } "
        "QLineEdit { "
        "   border: 2px solid #dee2e6; "
        "   border-radius: 6px; "
        "   padding: 8px 12px; "
        "   background-color: white; "
        "} "
        "QLineEdit:focus { border-color: #007bff; } "
        "QLabel { color: #495057; font-weight: 500; } "
        "QCheckBox { color: #6c757d; } "
        "QPlainTextEdit { "
        "   border: 2px solid #dee2e6; "
        "   border-radius: 6px; "
        "   background-color: #f8f9fa; "
        "   font-family: 'Consolas', 'Monaco', monospace; "
        "}"
    );
    
    // Make window more compact
    setMaximumSize(800, 500);
    setMinimumSize(600, 400);
}

// Enhanced USB device detection
QStringList MainWindow::getRemovableDevices()
{
    QStringList devices;
    QProcess process;
    
    // Enhanced lsblk command for better USB detection
    QString cmd = "lsblk -ndo NAME,SIZE,MODEL,VENDOR,TYPE,HOTPLUG,RM -I 3,8,22,179,259 2>/dev/null";
    process.start("bash", QStringList() << "-c" << cmd);
    process.waitForFinished(5000);
    
    if (process.exitCode() == 0) {
        QString output = process.readAllStandardOutput();
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        
        for (const QString &line : lines) {
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (parts.count() >= 6) {
                QString device = parts.at(0);
                QString size = parts.at(1);
                QString hotplug = parts.at(5);
                QString removable = parts.at(6);
                
                // Check if device is USB/removable
                if (isUsbDevice(device) || hotplug == "1" || removable == "1") {
                    QString info = QString("%1 (%2)").arg(device, size);
                    if (parts.count() > 2 && !parts.at(2).isEmpty()) {
                        info += " " + parts.at(2);
                    }
                    if (parts.count() > 3 && !parts.at(3).isEmpty()) {
                        info += " " + parts.at(3);
                    }
                    devices << info;
                }
            }
        }
    }
    
    return devices;
}

bool MainWindow::isUsbDevice(const QString &device)
{
    QProcess process;
    
    // Method 1: Check /sys/block for removable attribute
    QString sysPath = QString("/sys/block/%1/removable").arg(device);
    QFile removableFile(sysPath);
    if (removableFile.open(QIODevice::ReadOnly)) {
        QString content = removableFile.readAll().trimmed();
        if (content == "1") {
            return true;
        }
    }
    
    // Method 2: Use udevadm for detailed device info
    process.start("udevadm", QStringList() << "info" << "--query=property" << "--name=" + device);
    process.waitForFinished(3000);
    
    if (process.exitCode() == 0) {
        QString output = process.readAllStandardOutput();
        if (output.contains("ID_BUS=usb") || output.contains("ID_USB_DRIVER=usb-storage")) {
            return true;
        }
    }
    
    // Method 3: Check device path for USB connection
    QString devicePath = QString("/sys/block/%1").arg(device);
    QFileInfo pathInfo(devicePath);
    if (pathInfo.exists()) {
        QString realPath = pathInfo.canonicalFilePath();
        if (realPath.contains("/usb")) {
            return true;
        }
    }
    
    return false;
}

// Build the option list to be passed to formatting script
QString MainWindow::buildOptionList()
{
    device = ui->comboBoxUsbList->currentText().split(" ").at(0);
    device = device.replace("(", "").replace(")", "");
    label = ui->lineEditFSlabel->text();
    QString partoption;
    QString options;

    QString format = ui->comboBoxDataFormat->currentText();
    if (format.contains("fat32")) {
        format = "vfat";
    }

    if (ui->comboBoxPartitionTableType->isEnabled()) {
        partoption = ui->comboBoxPartitionTableType->currentText().toLower();
    } else {
        partoption = "part";
    }

    QString authentication = "pkexec";

    if (!QFile::exists("/usr/bin/pkexec") && QFile::exists("/usr/bin/gksu")) {
        authentication = "gksu";
    }

    if (getuid() == 0) {
        authentication = "";
    }

       // Gunakan path absolut di sistem
    QString scriptPath = "/usr/local/lib/formatusb/formatusb_lib";

   if (!QFile::exists(scriptPath)) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Library file not found: %1").arg(scriptPath));
    return QString();  // ✅ return string kosong
}


    options = QString("%1 \"%2\" \"%3\" \"%4\" \"%5\" \"%6\"")
              .arg(authentication)
              .arg(scriptPath)
              .arg(device)
              .arg(format)
              .arg(label)
              .arg(partoption);

    
    options = options.trimmed();
    qDebug() << "Device:" << device << "Format:" << format << "Label:" << label;
    qDebug() << "Options:" << options;
    return options;
}

// cleanup environment when window is closed
void MainWindow::cleanup()
{
    QString log_name = "/tmp/formatusb.log";
    QFile::remove(log_name);
}

// build the USB list with improved detection
QStringList MainWindow::buildUsbList()
{
    if (ui->checkBoxshowpartitions->isChecked()) {
        QString drives = cmd->getCmdOut("lsblk -nlo NAME,SIZE,LABEL,TYPE -I 3,8,22,179,259 |grep -v disk");
        return removeUnsuitable(drives.split("\n"));
    } else {
        return removeUnsuitable(getRemovableDevices());
    }
}

// remove unsuitable drives from the list (live and unremovable)
QStringList MainWindow::removeUnsuitable(const QStringList &devices)
{
    QStringList list;
    QString name;
    bool showall = ui->checkBoxShowAll->isChecked();
    
    for (const QString &line : devices) {
        if (line.trimmed().isEmpty()) continue;
        
        name = line.split(" ").at(0);
        name = name.replace("(", "").replace(")", "");
        
        if (!showall) {
            if (isUsbDevice(name)) {
                // Additional safety check - don't list system drives
                if (!isSystemDrive(name)) {
                    list << line;
                }
            }
        } else {
            if (!isSystemDrive(name)) {
                list << line;
            }
        }
    }
    
    return list;
}

bool MainWindow::isSystemDrive(const QString &device)
{
    QProcess process;
    
    // Check if device is mounted as root filesystem
    process.start("bash", QStringList() << "-c" << QString("mount | grep '/dev/%1' | grep ' / '").arg(device));
    process.waitForFinished(3000);
    
    if (process.exitCode() == 0 && !process.readAllStandardOutput().isEmpty()) {
        return true; // This is the root filesystem
    }
    
    // Check if device contains boot partitions
    process.start("bash", QStringList() << "-c" << QString("mount | grep '/dev/%1' | grep '/boot'").arg(device));
    process.waitForFinished(3000);
    
    if (process.exitCode() == 0 && !process.readAllStandardOutput().isEmpty()) {
        return true; // This contains boot partition
    }
    
    return false;
}

void MainWindow::cmdStart()
{
    setCursor(QCursor(Qt::WaitCursor));
    ui->buttonNext->setEnabled(false);
    ui->buttonBack->setEnabled(false);
}

void MainWindow::cmdDone()
{
    setCursor(QCursor(Qt::ArrowCursor));
    ui->buttonBack->setEnabled(true);
    
    if (cmd && cmd->exitCode() == 0 && cmd->exitStatus() == QProcess::NormalExit) {
       QMessageBox::information(this, tr("Success"),
    tr("USB device has been formatted successfully!\n\nYou can now safely remove the device.\n\nThank you for using this tool!"));

        
        // Refresh device list
        ui->comboBoxUsbList->clear();
        ui->comboBoxUsbList->addItems(buildUsbList());
    } else {
        QString errorMsg = tr("Error occurred during formatting process.");
        if (cmd) {
            QString stderr_output = cmd->readAllStandardError();
            if (!stderr_output.isEmpty()) {
                errorMsg += "\n\nDetails:\n" + stderr_output;
            }
        }
        QMessageBox::critical(this, tr("Formatting Failed"), errorMsg);
    }
    
    if (cmd) {
        cmd->disconnect();
    }
}

void MainWindow::setConnections()
{
    if (cmd) {
        connect(cmd, &QProcess::readyRead, this, &MainWindow::updateOutput);
        connect(cmd, &QProcess::started, this, &MainWindow::cmdStart);
        connect(cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), 
                this, &MainWindow::cmdDone);
    }
}

void MainWindow::updateOutput()
{
    if (!cmd) return;
    
    QString out = cmd->readAllStandardOutput();
    
    // Clean ANSI escape sequences for better display
    QRegularExpression ansiEscape("\\x1b\\[[0-9;]*[mK]");
    out.remove(ansiEscape);
    out.remove(QRegularExpression("\\x1b\\]0;[^\\x07]*\\x07"));
    
    ui->outputBox->moveCursor(QTextCursor::End);
    ui->outputBox->insertPlainText(out);
    QScrollBar *sb = ui->outputBox->verticalScrollBar();
    sb->setValue(sb->maximum());
    QApplication::processEvents();
}

// Next button clicked
void MainWindow::on_buttonNext_clicked()
{
    // on first page
    if (ui->stackedWidget->currentIndex() == 0) {
        if (ui->comboBoxUsbList->currentText().isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("Please select a USB device to format"));
            return;
        }

        // Enhanced confirmation dialog
        QString deviceInfo = ui->comboBoxUsbList->currentText();
        QString msg = tr("WARNING: This action will PERMANENTLY DESTROY all data on:\n\n")
                      + deviceInfo + "\n\n" 
                      + tr("Format: %1\nLabel: %2\n\n").arg(
                          ui->comboBoxDataFormat->currentText(),
                          ui->lineEditFSlabel->text()
                      )
                      + tr("Are you absolutely sure you want to continue?");
        
        QMessageBox::StandardButton reply = QMessageBox::warning(
            this, "Confirm USB Format", msg, 
            QMessageBox::Yes | QMessageBox::No, 
            QMessageBox::No
        );
        
        if (reply != QMessageBox::Yes) {
            return;
        }
        
        if (cmd && cmd->state() != QProcess::NotRunning) {
            ui->stackedWidget->setCurrentWidget(ui->outputPage);
            return;
        }
        
        ui->buttonBack->setHidden(false);
        ui->buttonBack->setEnabled(false);
        ui->buttonNext->setEnabled(false);
        ui->stackedWidget->setCurrentWidget(ui->outputPage);
        ui->outputBox->clear();
        ui->outputBox->appendPlainText("Starting USB formatting process...\n");

        QString options = buildOptionList();
        if (options.isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to build formatting options."));
            on_buttonBack_clicked();
            return;
        }
        
        makeUsb(options);
    }
}

void MainWindow::on_buttonBack_clicked()
{
    this->setWindowTitle("USB FORMAT v" + QString(VERSION));
    ui->stackedWidget->setCurrentIndex(0);
    ui->buttonNext->setEnabled(true);
    ui->buttonBack->setDisabled(true);
    ui->outputBox->clear();
    
    // Stop any running processes
    if (cmd && cmd->state() != QProcess::NotRunning) {
        cmd->kill();
        cmd->waitForFinished(3000);
    }
}

// About button clicked
void MainWindow::on_buttonAbout_clicked()
{
    this->hide();
    displayAboutMsgBox(tr("About %1").arg(this->windowTitle()),
                       "<p align=\"center\"><b><h2>USB Format</h2></b></p>"
                       "<p align=\"center\">" + tr("Version: ") + VERSION + "</p>"
                       "<p align=\"center\"><h3>" + tr("USB formatting tool") + "</h3></p>"
                       "<p align=\"center\">" + tr("Enhanced USB device detection and formatting") + "</p>"
                       "<p align=\"center\"><a href=\"https://github.com/WHO-AM-I-404/formatUSB\">GitHub Repository</a></p>"
                       "<p align=\"center\">" + tr("Copyright (C) 2025 FormatUSB Team") + "<br /><br /></p>",
                       "/usr/share/doc/formatusb/license.html", tr("%1 License").arg(this->windowTitle()), true);
    this->show();
}

// Help button clicked
void MainWindow::on_buttonHelp_clicked()
{
    QString url = "https://github.com/WHO-AM-I-404/FormatUSB/edit/main/README.md";
    displayDoc(url, tr("%1 Help").arg(this->windowTitle()), true);
}

void MainWindow::on_buttonRefresh_clicked()
{
    ui->buttonRefresh->setEnabled(false);
    ui->buttonRefresh->setText(tr("Detecting..."));
    
    // Add a small delay to show user that detection is happening
    QTimer::singleShot(500, this, [this]() {
        ui->comboBoxUsbList->clear();
        ui->comboBoxUsbList->addItems(buildUsbList());
        ui->buttonRefresh->setEnabled(true);
        ui->buttonRefresh->setText(tr("Refresh"));
    });
}

void MainWindow::on_checkBoxShowAll_clicked()
{
    on_buttonRefresh_clicked();
}

void MainWindow::on_checkBoxshowpartitions_clicked()
{
    on_buttonRefresh_clicked();
    ui->comboBoxPartitionTableType->setEnabled(!ui->checkBoxshowpartitions->isChecked());
}

void MainWindow::validate_name()
{
    QString test = ui->lineEditFSlabel->text();
    
    if (test.isEmpty()) {
        ui->buttonNext->setEnabled(true);
        return;
    }
    
    QString regexstring;
    QString format = ui->comboBoxDataFormat->currentText();
    
    if (format == "fat32") {
        regexstring = "^[A-Za-z0-9_-]{1,11}$"; // FAT32 label restrictions
    } else if (format == "ext4") {
        regexstring = "^[A-Za-z0-9_.-]{1,16}$"; // ext4 label restrictions
    } else if (format == "ntfs") {
        regexstring = "^[A-Za-z0-9_. -]{1,32}$"; // NTFS allows spaces
    } else if (format == "exfat") {
        regexstring = "^[A-Za-z0-9_. -]{1,15}$"; // exFAT restrictions
    } else {
        regexstring = "^[A-Za-z0-9_.-]{1,16}$"; // Default
    }
    
    QRegularExpression regex(regexstring);
    if (!regex.match(test).hasMatch()) {
        QMessageBox::warning(this, tr("Invalid Label"), 
            tr("The volume label contains invalid characters or is too long.\n\n")
            + tr("Allowed characters: A-Z, a-z, 0-9, underscore, hyphen") 
            + (format == "ntfs" || format == "exfat" ? tr(", space, period") : ""));
        ui->buttonNext->setEnabled(false);
    } else {
        ui->buttonNext->setEnabled(true);
    }
}

void MainWindow::on_lineEditFSlabel_textChanged(const QString &)
{
    validate_name();
}

void MainWindow::on_comboBoxDataFormat_currentIndexChanged(int)
{
    validate_name();
}
