#ifndef TCPFILESERVERANDSENDER_H
#define TCPFILESERVERANDSENDER_H

#include <QWidget>
#include "tcpfilesender.h"
#include "tcpfileserver.h"

class TcpFileServerandSender : public QWidget
{
    Q_OBJECT

public:
    explicit TcpFileServerandSender(QWidget *parent = nullptr);

private:
    TcpFileSender *sender;
    TcpFileServer *receiver;
};

#endif // TCPFILESERVERANDSENDER_H
