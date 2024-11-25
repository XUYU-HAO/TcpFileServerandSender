#include "TcpFileServerandSender.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

TcpFileServerandSender::TcpFileServerandSender(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *instructionLabel = new QLabel(QStringLiteral("選擇要執行的功能："));
    layout->addWidget(instructionLabel);

    QPushButton *sendButton = new QPushButton(QStringLiteral("啟動檔案傳送"));
    QPushButton *receiveButton = new QPushButton(QStringLiteral("啟動檔案接收"));
    layout->addWidget(sendButton);
    layout->addWidget(receiveButton);

    QLabel *statusLabel = new QLabel(QStringLiteral("狀態：等待選擇..."));
    layout->addWidget(statusLabel);

    sender = new TcpFileSender(this);
    receiver = new TcpFileServer(this);

    connect(sendButton, &QPushButton::clicked, this, [this, statusLabel]() {
        sender->show();
        statusLabel->setText(QStringLiteral("檔案傳送介面已啟動"));
    });

    connect(receiveButton, &QPushButton::clicked, this, [this, statusLabel]() {
        receiver->show();
        statusLabel->setText(QStringLiteral("檔案接收介面已啟動"));
    });

    setLayout(layout);
    setWindowTitle(QStringLiteral("檔案傳輸應用"));
}
