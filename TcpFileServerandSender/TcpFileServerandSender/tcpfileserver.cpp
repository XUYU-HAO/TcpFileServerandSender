#include "tcpfileserver.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHostAddress>
#include <QDataStream>
#include <QFile>
#include <QByteArray>
#include <QDebug>

TcpFileServer::TcpFileServer(QWidget *parent)
    : QDialog(parent)
{
    totalBytes = 0;
    byteReceived = 0;
    fileNameSize = 0;

    serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(QStringLiteral("伺服器端就緒"));
    startButton = new QPushButton(QStringLiteral("接收"));
    quitButton = new QPushButton(QStringLiteral("退出"));
    ipLineEdit = new QLineEdit;  // 用戶輸入 IP 地址
    portLineEdit = new QLineEdit;  // 用戶輸入端口號

    // 預設 IP 和端口
    ipLineEdit->setPlaceholderText("請輸入伺服器 IP 地址");
    portLineEdit->setPlaceholderText("請輸入伺服器端口號");
    portLineEdit->setText("16998");  // 預設端口 16998

    // 初始化 buttonBox
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addWidget(new QLabel(QStringLiteral("伺服器 IP:")));
    mainLayout->addWidget(ipLineEdit);
    mainLayout->addWidget(new QLabel(QStringLiteral("伺服器端口:")));
    mainLayout->addWidget(portLineEdit);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(QStringLiteral("接收檔案"));

    connect(startButton, &QPushButton::clicked, this, &TcpFileServer::start);
    connect(quitButton, &QPushButton::clicked, this, &TcpFileServer::close);
    connect(&tcpServer, &QTcpServer::newConnection, this, &TcpFileServer::acceptConnection);
    connect(&tcpServer, &QTcpServer::acceptError, this, &TcpFileServer::displayError);
}

TcpFileServer::~TcpFileServer()
{
    if (localFile && localFile->isOpen()) {
        localFile->close();
    }
}

void TcpFileServer::start()
{
    startButton->setEnabled(false);
    byteReceived = 0;
    fileNameSize = 0;

    QString ipAddress = ipLineEdit->text();  // 取得用戶輸入的 IP
    quint16 port = portLineEdit->text().toUInt();  // 取得用戶輸入的端口號

    if (ipAddress.isEmpty() || port == 0) {
        QMessageBox::warning(this, QStringLiteral("輸入錯誤"), QStringLiteral("請輸入有效的 IP 地址和端口號。"));
        startButton->setEnabled(true);
        return;
    }

    while (!tcpServer.isListening() &&
           !tcpServer.listen(QHostAddress(ipAddress), port)) {
        QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                                                QStringLiteral("伺服器錯誤"),
                                                                QStringLiteral("無法啟動伺服器: %1.").arg(tcpServer.errorString()),
                                                                QMessageBox::Retry | QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel) {
            startButton->setEnabled(true);
            return;
        }
    }

    serverStatusLabel->setText(QStringLiteral("伺服器監聽中..."));
}

void TcpFileServer::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();  // 接收客戶端的連線
    connect(tcpServerConnection, &QTcpSocket::readyRead, this, &TcpFileServer::updateServerProgress);
    connect(tcpServerConnection, &QTcpSocket::errorOccurred, this, &TcpFileServer::displayError);  // 修正此行
    serverStatusLabel->setText(QStringLiteral("已接受連線"));
    tcpServer.close();  // 停止接受新的連線
}

void TcpFileServer::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_6);
    if (byteReceived <= sizeof(qint64) * 2)
    {
        if ((fileNameSize == 0) && (tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2))
        {
            in >> totalBytes >> fileNameSize;
            byteReceived += sizeof(qint64) * 2;
        }
        if ((fileNameSize != 0) && (tcpServerConnection->bytesAvailable() >= fileNameSize))
        {
            in >> fileName;
            byteReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly))
            {
                QMessageBox::warning(this, QStringLiteral("應用程式"),
                                     QStringLiteral("無法創建檔案 %1：\n%2.").arg(fileName)
                                         .arg(localFile->errorString()));
                return;
            }
        }
        else
        {
            return;
        }
    }

    if (byteReceived < totalBytes)
    {
        byteReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    serverProgressBar->setMaximum(totalBytes);
    serverProgressBar->setValue(byteReceived);
    serverStatusLabel->setText(QStringLiteral("已接收 %1 Bytes").arg(byteReceived));

    if (byteReceived == totalBytes)
    {
        tcpServerConnection->close();
        startButton->setEnabled(true);
        localFile->close();
        start();
    }
}

void TcpFileServer::displayError(QAbstractSocket::SocketError socketError)
{
    QMessageBox::information(this, QStringLiteral("網絡錯誤"),
                             QStringLiteral("發生錯誤: %1").arg(tcpServerConnection->errorString()));
    tcpServerConnection->close();
    serverProgressBar->reset();
    serverStatusLabel->setText(QStringLiteral("伺服器就緒"));
    startButton->setEnabled(true);
}
