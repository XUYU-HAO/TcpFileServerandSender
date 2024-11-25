#ifndef TCPFILESERVER_H
#define TCPFILESERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>

class TcpFileServer : public QDialog
{
    Q_OBJECT

public:
    explicit TcpFileServer(QWidget *parent = nullptr);
    ~TcpFileServer();

private slots:
    void start();  // 啟動伺服器
    void acceptConnection();  // 接受來自客戶端的連線
    void updateServerProgress();  // 更新接收檔案的進度
    void displayError(QAbstractSocket::SocketError socketError);  // 顯示錯誤訊息

private:
    QTcpServer tcpServer;  // TCP伺服器
    QTcpSocket *tcpServerConnection;  // 用來處理與客戶端的連線
    QFile *localFile;  // 用來儲存接收到的檔案
    QProgressBar *serverProgressBar;  // 顯示接收進度的進度條
    QLabel *serverStatusLabel;  // 顯示伺服器狀態的標籤
    QPushButton *startButton;  // 啟動伺服器按鈕
    QPushButton *quitButton;  // 退出按鈕
    QLineEdit *ipLineEdit;  // 用戶輸入IP地址
    QLineEdit *portLineEdit;  // 用戶輸入端口號
    QDialogButtonBox *buttonBox;  // 按鈕框
    qint64 totalBytes;  // 總的檔案大小
    qint64 byteReceived;  // 已接收到的檔案大小
    qint64 fileNameSize;  // 檔名長度
    QString fileName;  // 檔名
    QByteArray inBlock;  // 存放讀取資料的緩衝區
};

#endif // TCPFILESERVER_H
