#include "cmd.h"

#include <QDebug>
#include <QEventLoop>

Cmd::Cmd(QObject *parent)
    : QProcess(parent)
{
    connect(this, &Cmd::readyReadStandardOutput, [=]() {
        emit outputAvailable(readAllStandardOutput());
    });
    connect(this, &Cmd::readyReadStandardError, [=]() {
        emit errorAvailable(readAllStandardError());
    });

    connect(this, &Cmd::outputAvailable, [=](const QString &out) {
        out_buffer.append(out.split('\n', Qt::SkipEmptyParts));
    });
    connect(this, &Cmd::errorAvailable, [=](const QString &out) {
        out_buffer.append(out.split('\n', Qt::SkipEmptyParts));
    });
}

void Cmd::halt()
{
    if (state() != QProcess::NotRunning) {
        terminate();
        if (!waitForFinished(5000)) {
            kill();
            waitForFinished(1000);
        }
    }
}

bool Cmd::run(const QString &cmd, bool quiet)
{
    QString output;
    return run(cmd, output, quiet);
}

QString Cmd::getCmdOut(const QString &cmd, bool quiet)
{
    QString output;
    run(cmd, output, quiet);
    return output;
}

bool Cmd::run(const QString &cmd, QString &output, bool quiet)
{
    out_buffer.clear();

    if (state() != QProcess::NotRunning) {
        qDebug() << "Process already running:" << program() << arguments();
        return false;
    }

    if (!quiet) qDebug().noquote() << cmd;

    QEventLoop loop;
    connect(this, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Cmd::cmdFinished, Qt::UniqueConnection);
    connect(this, &Cmd::cmdFinished, &loop, &QEventLoop::quit, Qt::UniqueConnection);

    start("/bin/bash", {"-c", cmd});
    loop.exec();

    output = out_buffer.join("\n").trimmed();
    return (exitStatus() == QProcess::NormalExit && exitCode() == 0);
}
