#include "TcpFileServerandSender.h"

TcpFileServerandSender::TcpFileServerandSender(QWidget *parent)
    : QWidget(parent), sender(new TcpFileSender(this)), receiver(new TcpFileServer(this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tabWidget = new QTabWidget(this);

    setupFileSenderTab();
    setupFileReceiverTab();

    tabWidget->addTab(fileSenderTab, QStringLiteral("檔案傳送"));
    tabWidget->addTab(fileReceiverTab, QStringLiteral("檔案接收"));

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    setWindowTitle(QStringLiteral("檔案傳輸系統"));
}

void TcpFileServerandSender::setupFileSenderTab()
{
    fileSenderTab = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(fileSenderTab);
    QLabel *statusLabel = new QLabel(QStringLiteral("等待操作..."), fileSenderTab);
    QPushButton *sendButton = new QPushButton(QStringLiteral("啟動傳送"), fileSenderTab);

    layout->addWidget(statusLabel);
    layout->addWidget(sendButton);

    connect(sendButton, &QPushButton::clicked, this, [this, statusLabel]() {
        sender->show();
        statusLabel->setText(QStringLiteral("檔案傳送介面已啟動"));
    });

    fileSenderTab->setLayout(layout);
}

void TcpFileServerandSender::setupFileReceiverTab()
{
    fileReceiverTab = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(fileReceiverTab);
    QLabel *statusLabel = new QLabel(QStringLiteral("等待操作..."), fileReceiverTab);
    QPushButton *receiveButton = new QPushButton(QStringLiteral("啟動接收"), fileReceiverTab);

    layout->addWidget(statusLabel);
    layout->addWidget(receiveButton);

    connect(receiveButton, &QPushButton::clicked, this, [this, statusLabel]() {
        receiver->show();
        statusLabel->setText(QStringLiteral("檔案接收介面已啟動"));
    });

    fileReceiverTab->setLayout(layout);
}
