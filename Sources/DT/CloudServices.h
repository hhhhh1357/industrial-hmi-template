#ifndef CLOUDSERVICES_H
#define CLOUDSERVICES_H

#include "CloudTalker.h"

#include <QObject>
#include <QThread>
#include <QMap>

class CloudServices : public QObject
{
    Q_OBJECT
public:
    explicit CloudServices(QList<QString> ipAndPorts, QObject *parent = nullptr);
    ~CloudServices();

    QList<QThread*> threadList;   //线程队列
    QList<CloudTalker*> cloudTalkerList;  //云端线程队列

//    QTimer *deleteWorkerTimer = nullptr;

//    QTimer *heartBeatTimer=nullptr;    //定时发心跳包
//    QTimer *checkServerTimer=nullptr;  //定时Ping服务端
//    QTimer *reportTimer=nullptr;  //定时上报状态
//    QTimer *checkSocketTimer;          //定时重新连接Server Socket

    QList<QTimer*> deleteWorkerTimerList;

    QList<QTimer*> heartBeatTimerList;    //定时发心跳包
    QList<QTimer*> checkServerTimerList;  //定时Ping服务端
    QList<QTimer*> reportTimerList;  //定时上报状态
    QList<QTimer*> checkSocketTimerList;          //定时重新连接Server Socket



private:
    int cloudTalkerCount=0;   //云端线程数量
public slots:
    void handleConnectionState(ConnectStatus talkerConnectionState);  //接收并处理CloudTalker返回的信息
    //void handleConnectionState();  //接收并处理CloudTalker返回的信息
    void handRequest(Task task);    //接收调用者请求 槽
    void handResult(QString msg);  //处理返回的信息 槽

    //void selfReport(QString msg);


signals:

    void dispatchTask(Task task);

    void reportConnectionState(ConnectStatus connectionState);  //向UI报告连接状态 信号
    //void reportConnectionState();
    void startCloudTalker(); //启动云端对话
    void stopCloudTalker();  //停止云端对话
    void requestCloudTalker(Task task);   //向云端发送请求信号
    void reportResult(QString msg);   //向调用者报告返回的信息
};








#endif // CLOUDSERVICES_H
