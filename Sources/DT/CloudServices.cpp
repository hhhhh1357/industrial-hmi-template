#include "CloudServices.h"
#include "ConnectStatus.h"

#include "plc.h"

#include <QThread>
#include <QProcess>

#include <Headers/BLL/BSysDictionary.h>

extern PLC plc;
CloudServices::CloudServices(QList<QString> ipAndPorts, QObject *parent)
{
    //QList<Server> ServerList;  localService.ServerList
    //===========================================================================================================================
//    deleteWorkerTimer=new QTimer();
//    deleteWorkerTimer->setInterval(3600*1000);
//    //    //定时向服务器大吼一声
//        heartBeatTimer = new QTimer();
//        heartBeatTimer->setInterval(5000);

//    //    //定时检测网卡是否启用
//        checkServerTimer = new QTimer();
//        checkServerTimer->setInterval(20*1000);//一分钟

//        //定时上报状态
//        reportTimer = new QTimer();
//        reportTimer->setInterval(10000);    //

//        checkSocketTimer = new QTimer();
//        checkSocketTimer->setInterval(30*1000);   //每 30s 一次检查连接
        //===========================================================================================================================

    //qDebug()<<__FILE__<<", "<<__func__<<",线程ID:"<<QThread::currentThread()<<endl;
    QString ipAndPort = "";
    int totalNum = ipAndPorts.count();
    for(int i=0; i<totalNum; i++){
        ipAndPort = ipAndPorts.at(i);
        //qDebug()<<"ipAndPort: "<<ipAndPort<<endl;
        this->cloudTalkerCount++;

        /////////////////////////////
        QTimer* deleteWorkerTimer=new QTimer();
        deleteWorkerTimer->setInterval(3600*1000);
        deleteWorkerTimerList.append(deleteWorkerTimer);
        //    //定时向服务器大吼一声
         QTimer* heartBeatTimer = new QTimer();
            heartBeatTimer->setInterval(30*1000);
            heartBeatTimerList.append(heartBeatTimer);

        //    //定时检测网卡是否启用
//        QTimer* checkServerTimer = new QTimer();
//            checkServerTimer->setInterval(5000);
//            checkServerTimerList.append(checkServerTimer);

            //定时上报状态
         QTimer* reportTimer = new QTimer();
            reportTimer->setInterval(5000);
            reportTimerList.append(reportTimer);

        QTimer* checkSocketTimer = new QTimer();
            SysDictionary sysDic;
            checkSocketTimer->setInterval(sysDic.getPING_SERVERS_Interval()*1000); //根据配置的时间设置定时器
            //checkSocketTimer->setInterval(10000);   //每 30s 一次检查连接
            checkSocketTimerList.append(checkSocketTimer);
        /// //////////////////



        CloudTalker *cloudTalker=new CloudTalker(ipAndPort);
        connect(deleteWorkerTimer, &QTimer::timeout, cloudTalker, &CloudTalker::deleteDeadWorkerSlot, Qt::UniqueConnection);

        connect(heartBeatTimer, SIGNAL(timeout()), cloudTalker, SLOT(heartBeat()), Qt::UniqueConnection);
//        connect(checkServerTimer, &QTimer::timeout, cloudTalker, &CloudTalker::checkServerStatusSlot, Qt::UniqueConnection);
        connect(reportTimer, &QTimer::timeout, cloudTalker, &CloudTalker::periodicalReportServerStatusSlot, Qt::UniqueConnection);
        connect(checkSocketTimer, &QTimer::timeout, cloudTalker, &CloudTalker::socketReConnnectSlot, Qt::UniqueConnection);

        cloudTalkerList.append(cloudTalker);
        //===========================================================================================================================
        QThread *t = new QThread(this);
        threadList.append(t);

        cloudTalker->moveToThread(t);
        //qDebug()<<"cloudTalker->moveToThread(t)----------------------->"<<endl;

        deleteWorkerTimer->moveToThread(t);
        //qDebug()<<"deleteWorkerTimer->moveToThread(t)-------------------->"<<endl;

        heartBeatTimer->moveToThread(t);
        //qDebug()<<"heartBeatTimer->moveToThread(t)------------------------>"<<endl;
//        checkServerTimer->moveToThread(t);
        //qDebug()<<"checkServerTimer->moveToThread(t)----------------------->"<<endl;
        reportTimer->moveToThread(t);
        //qDebug()<<"reportTimer->moveToThread(t)------------------------->"<<endl;
        checkSocketTimer->moveToThread(t);
        //qDebug()<<"checkSocketTimer->moveToThread(t)-------------------->"<<endl;

        connect(t, SIGNAL(started()), deleteWorkerTimer, SLOT(start()));
        connect(this, SIGNAL(startCloudTalker()), heartBeatTimer, SLOT(start()));
//        connect(this, SIGNAL(startCloudTalker()), checkServerTimer, SLOT(start()));
        connect(this, SIGNAL(startCloudTalker()), reportTimer, SLOT(start()));
        connect(this, SIGNAL(startCloudTalker()), checkSocketTimer, SLOT(start()));

        connect(t, &QThread::finished, cloudTalker, &QObject::deleteLater);
        //===========================================================================================================================
        connect(this, &CloudServices::startCloudTalker, cloudTalker, &CloudTalker::startCloudTalker);
        connect(this, &CloudServices::stopCloudTalker, cloudTalker, &CloudTalker::stopCloudTalker);
        connect(this, &CloudServices::requestCloudTalker, cloudTalker, &CloudTalker::handleRequestCloudTalker);

        connect(cloudTalker, &CloudTalker::reportConnectionState, this, &CloudServices::handleConnectionState);

        connect(this, &CloudServices::dispatchTask, cloudTalker, &CloudTalker::taskDispatcher);


        //接收PLC发送的消息连接
        connect(&plc, &PLC::isCloudRequestDone, cloudTalker, &CloudTalker::handleRequestCloudTalker);
        //cloudTalker处理完成，请求一个槽函数处理返回结果， connect what ?
        //qDebug()<<__FILE__<<", "<<__func__<<",线程ID:"<<QThread::currentThread()<<endl;

        //===========================================================================================================================
        t->start();  //启动线程

        //=========================================
        //qDebug()<<__FILE__ << "---" <<__func__<<", call startCloudTalker() !"<<endl;
       emit startCloudTalker();
        //cloudTalker->startCloudTalker();
        //sleep(500);
    }

}
CloudServices::~CloudServices()
{
    //qDebug()<<__FILE__<<", " <<__func__<<",=====> "<<endl;
    foreach(QThread *t, threadList){
        t->quit();
        t->wait();
    }
    foreach(QTimer* t , heartBeatTimerList){
        delete t;
    }
    foreach(QTimer* t , checkServerTimerList){
        delete t;
    }
    foreach(QTimer* t , checkSocketTimerList){
        delete t;
    }
    foreach(QTimer* t , reportTimerList){
        delete t;
    }
    foreach(QTimer* t , deleteWorkerTimerList){
        delete t;
    }

//    if(heartBeatTimer!=nullptr){
//        delete heartBeatTimer;
//        heartBeatTimer = nullptr;
//    }
//    if(checkServerTimer!=nullptr){
//         delete checkServerTimer;
//        checkServerTimer = nullptr;
//    }
//    if(checkSocketTimer!=nullptr){
//         delete checkSocketTimer;
//        checkSocketTimer = nullptr;
//    }
//    if(reportTimer!=nullptr){
//        delete reportTimer;
//        reportTimer = nullptr;
//    }

}
//接收并处理CloudTalker返回的连接状态信息
void CloudServices::handleConnectionState(ConnectStatus connectionStates)
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    emit reportConnectionState(connectionStates); //继续向上(UI)报告连接状态信息
}

void CloudServices::handRequest(Task task)
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<task.dataPacket.data.SelfReportCmd<<endl;
    emit requestCloudTalker(task);
}

void CloudServices::handResult(QString msg)  //处理返回的信息 槽
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    emit reportResult(msg);
}
//void CloudServices::selfReport(QString msg)
//{
//    PackingTools packingTools;
//    DataPacketData data;
//    //调用拆包函数，将msg分解,生成DataPacketData对象data
//    CloudDataPacket cdp = packingTools.CloudDataPacketBuilder(msg);

//    Task task;
//    task.taskSource = TaskDirection::SELF_REPORT;
//    task.dataPacket = cdp;
//    qDebug()<<__FILE__<<",  "<<__func__<<"=================>  selfReport(QString msg), msg="<<msg<<endl;
//    emit dispatchTask(task);
//}
