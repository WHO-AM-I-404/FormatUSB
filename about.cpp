#include "about.h"
#include "cmd.h"
#include "version.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDialog>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>
#include <unistd.h>

// display doc as normal user when run as root
void displayDoc(QString url, QString title, bool runned_as_root)
{
    // Use standard xdg-open for cross-platform compatibility
    if (getuid() != 0) {
        QString cmd = "xdg-open " + url;
        system(cmd.toUtf8());
    } else {
        system("su $(logname) -c \"env XDG_RUNTIME_DIR=/run/user/$(id -u $(logname)) xdg-open "
               + url.toUtf8() + "\"&");
    }
}

void displayAboutMsgBox(QString title, QString message, QString licence_url,
                        QString license_title, bool runned_as_root)
{
    QMessageBox msgBox(QMessageBox::NoIcon, title, message);
    QPushButton *btnLicense   = msgBox.addButton(QObject::tr("License"), QMessageBox::HelpRole);
    QPushButton *btnChangelog = msgBox.addButton(QObject::tr("Changelog"), QMessageBox::HelpRole);
    QPushButton *btnCancel    = msgBox.addButton(QObject::tr("Cancel"), QMessageBox::NoRole);
    btnCancel->setIcon(QIcon::fromTheme("window-close"));

    msgBox.exec();

    if (msgBox.clickedButton() == btnLicense) {
        displayDoc(licence_url, license_title, runned_as_root);
    } else if (msgBox.clickedButton() == btnChangelog) {
        QDialog *changelog = new QDialog();
        changelog->setWindowTitle(QObject::tr("Changelog"));
        changelog->resize(600, 500);

        QTextEdit *text = new QTextEdit;
        text->setReadOnly(true);

        // Baca file CHANGELOG.txt di folder aplikasi
        QFile file(QCoreApplication::applicationDirPath() + "/CHANGELOG.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            text->setText(QString::fromUtf8(file.readAll()));
            file.close();
        } else {
            text->setText(QObject::tr("CHANGELOG.txt not found."));
        }

        QPushButton *btnClose = new QPushButton(QObject::tr("&Close"));
        btnClose->setIcon(QIcon::fromTheme("window-close"));
        QObject::connect(btnClose, &QPushButton::clicked, changelog, &QDialog::close);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(text);
        layout->addWidget(btnClose);
        changelog->setLayout(layout);
        changelog->exec();
        delete changelog;
    }
}
