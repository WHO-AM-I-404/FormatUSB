#pragma once

#include <QProcess>
#include <QString>
#include <QStringList>

class Cmd : public QProcess
{
    Q_OBJECT
public:
    explicit Cmd(QObject *parent = nullptr);

    // Hentikan proses yang sedang berjalan
    void halt();

    // Jalankan perintah tunggal (string mentah)
    [[nodiscard]] bool run(const QString &cmd, bool quiet = false);

    // Jalankan perintah tunggal dan ambil output
    [[nodiscard]] bool run(const QString &cmd, QString &output, bool quiet = false);

    // Jalankan program dengan argumen (lebih aman daripada string mentah)
    [[nodiscard]] bool run(const QString &program, const QStringList &arguments, bool quiet = false);

    // Shortcut untuk dapatkan output langsung
    [[nodiscard]] QString getCmdOut(const QString &cmd, bool quiet = false);

signals:
    void cmdFinished();                       // selesai (dibungkus dari QProcess::finished)
    void errorAvailable(const QString &err);  // error output
    void outputAvailable(const QString &out); // standard output

private:
    QStringList out_buffer; // simpan output baris demi baris
};
