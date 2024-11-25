#include "tcpfilesender.h"
#include <QApplication>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpFileSender w;

    bool ok;
    QString ip = QInputDialog::getText(&w, QStringLiteral("輸入IP"), QStringLiteral("IP地址:"), QLineEdit::Normal, "127.0.0.1", &ok);
    if (!ok) return 0;

    int port = QInputDialog::getInt(&w, QStringLiteral("輸入Port"), QStringLiteral("Port號碼:"), 16998, 1, 65535, 1, &ok);
    if (!ok) return 0;

    w.setConnectionDetails(ip, port);
    w.show();

    return a.exec();
}
