#ifndef CLOUDTASKHANDLER_H
#define CLOUDTASKHANDLER_H

#include "CloudDataPacket.h"

#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include <QObject>
#include <qqueue.h>


class CloudTaskHandler : public QThread
{
    Q_OBJECT
public:
    explicit CloudTaskHandler(QObject *parent = nullptr);
    //CloudTaskHandler(QString serverIP, unsigned short port,QObject *parent=nullptr);

    ~CloudTaskHandler();
    //连接云端服务器
    void connectServer(unsigned short port, QString ip);
    //读取云端数据

    //向云端写数据

private:
    QString m_serverIP;
    unsigned short m_serverPort;
    bool m_connected;
    bool m_run;

    QTcpSocket *m_tcp;
    QQueue<CloudDataPacket> fromCloud;
    QQueue<CloudDataPacket> toCloud;

signals:


        // QThread interface
protected:
        void run();
};
/*
CloudTaskHandler::CloudTaskHandler(QObject *parent)
{
    fromCloud = new QQueue<CloudDataPacket>();
}*/
/*
CloudTaskHandler::CloudTaskHandler( QString serverIP, unsigned short port,QObject *parent):m_serverIP(serverIP),m_serverPort(port)
{
    setParent(parent);
    m_connected = false;
    m_run = true;

    m_tcp = new QTcpSocket(this);
    m_tcp->connectToHost(QHostAddress(serverIP), port);
    connect(m_tcp, &QTcpSocket::connected, this, [=](){
        m_connected = true;
    });
    connect(m_tcp, &QTcpSocket::disconnected, this,[=](){
        m_connected = false;
    });
}
*/

#endif // CLOUDTASKHANDLER_H

