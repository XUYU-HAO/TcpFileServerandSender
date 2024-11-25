#ifndef TCPFILESERVERANDSENDER_H
#define TCPFILESERVERANDSENDER_H

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "tcpfilesender.h"
#include "tcpfileserver.h"

class TcpFileServerandSender : public QWidget
{
    Q_OBJECT

public:
    TcpFileServerandSender(QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;
    QWidget *fileSenderTab;
    QWidget *fileReceiverTab;

    TcpFileSender *sender;
    TcpFileServer *receiver;

    void setupFileSenderTab();
    void setupFileReceiverTab();
};

#endif // TCPFILESERVERANDSENDER_H
