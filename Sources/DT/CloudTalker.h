#ifndef CLOUDTALKER_H
#define CLOUDTALKER_H

#include "DataPacketData.h"
#include "PackingTools.h"
#include "Worker.h"
#include "Task.h"
#include "ConnectStatus.h"

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QQueue>
#include <QMutex>
#include <QProcess>

class CloudTalker : public QObject
{
    Q_OBJECT
public:
    explicit CloudTalker(QString ipAndPort,QObject *parent = nullptr,int SubWorker_Duration=30);  // 根据冒号分解得到IP和Port,初始化一个云端对话对象

    ~CloudTalker();

private:
    bool m_netCardOk=false;

    QTcpSocket *socket=nullptr;
    QString ip;
    unsigned short port;
    QString MN;
    QString PW;

    int Duration=30;   //SubWorker对象最大存活时间（分钟）

    bool m_Ping_Ok = false;
    QMutex m_lock_ping;

    bool m_Network_Ok_last=true;  //上次的状态
    bool m_Network_Ok=true;   //ping不通服务器或者拔掉了网线，即为false;
    QMutex m_lock_Network;

    //QAbstractSocket::SocketState state;
    bool m_actualOpen = false;   //记录socket连接状态
    QMutex m_lock_actualOpen;

    bool m_shouldOpen = true;    //
    QMutex m_lock_shouldOpen;

    Worker *worker=nullptr;  //


    PackingTools packingTools;




signals:
    void taskDispatcher(Task task);   //分发数据到各个业务对象的信号

    void reportConnectionState(ConnectStatus connectionStates);   //向CloudService报告Socket状态

    void deleteDeadWorker(int afterMinutes);  //删除死Worker  信号
private slots:
    void handleCheckConnection();   //检查连接的心跳   槽
    void handleReadyRead();     //接收TCPSocket的数据包  槽（查找并分发数据到各个具体的业务对象）


    void handleStateChanged(QAbstractSocket::SocketState newValue);   //与Socket连接状态相关的 槽
    void connected();   //与Socket连接相关的 槽
    void disconnected();   //与Socket连接相关的 槽


    bool isNetworkCardEnabled();  //检查网卡是否启用
    bool pingServer();



public slots:
    void heartBeat();   //检查
    void checkServerStatusSlot();
    void periodicalReportServerStatusSlot();
    void socketReConnnectSlot();
    void deleteDeadWorkerSlot();

    void startCloudTalker();     //启动云端对话
    void stopCloudTalker();      //停止云端对话
    void handleRequestCloudTalker(Task task);    //接收主动上报数据包或UI请求的包      槽


    void handleToCloud(QString msg);   //接收内部发往云端的数据包  槽

};

#endif // CLOUDTALKER_H
