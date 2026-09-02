#include "CloudTalker.h"
#include "Sources/Tools/ping.h"

#include <QNetworkInterface>

#include <QThread>
#include <Headers/BLL/BLocalService.h>

extern BLocalService * localService;
CloudTalker::CloudTalker(QString ipAndPort,QObject *parent,int SubWorker_Duration) : QObject(parent),Duration(SubWorker_Duration)
{
    m_netCardOk = isNetworkCardEnabled();

    QStringList list = ipAndPort.split(":");

    ip = list.at(0);
    port = list.at(1).toUShort();
    MN = list.at(2);
    PW = list.at(3);
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<", IP="<<ip<<", Port="<<port<<endl;

    //worker = new Worker(ip,PW,MN);   //为哪个云端ip服务的Worker;
    //startCloudTalker();       //由CloudServices发送启动信号来启动云端对话；
//    connect(socket, &QTcpSocket::connected, this, &CloudTalker::connected);
//    connect(socket, &QTcpSocket::disconnected, this, &CloudTalker::disconnected);
//    connect(socket, &QTcpSocket::stateChanged, this, &CloudTalker::handleStateChanged);
//    connect(socket, &QTcpSocket::readyRead, this, &CloudTalker::handleReadyRead,Qt::ConnectionType::DirectConnection);   //接收socket数据报
//    connect(this, &CloudTalker::taskDispatcher, worker, &Worker::doWork );  //分配任务干活
//    connect(worker, &Worker::toCloud, this, &CloudTalker::handleToCloud);     //接收Worker的toCloud信号，

}

CloudTalker::~CloudTalker()
{
    if(worker!=nullptr){
        delete  worker;
        worker = nullptr;
    }
}
void CloudTalker::handleCheckConnection(){ //检查连接的心跳函数  槽
    //socket.state
}
void CloudTalker::handleReadyRead()     //接收TCPSocket的数据包  槽（查找并分发数据到各个具体的业务对象）
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    QByteArray allData = socket->readAll();
    QString msg = QString(allData);
    DataPacketData data;
    //调用拆包函数，将msg分解,生成DataPacketData对象data
    CloudDataPacket cdp = packingTools.CloudDataPacketBuilder(msg);
    //data = cdp.data;
    Task task;
    task.taskSource = TaskDirection::CLOUD_REQUEST;
    task.dataPacket = cdp;
    //qDebug()<<__FILE__<<",  "<<__func__<<"=================>  handleReadyRead..., msg="<<msg<<endl;
    //qDebug()<<this->socket->peerAddress()<<endl;
    emit taskDispatcher(task);
}
//接收主动上报、UI上报的数据包请求 槽（查找并分发数据到各个具体的业务对象）

void CloudTalker::handleRequestCloudTalker(Task task){
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    emit taskDispatcher(task);
}
void CloudTalker::handleStateChanged(QAbstractSocket::SocketState newValue)   //与Socket连接状态相关的 槽
{
    //state = newValue;
    switch(newValue)
    {
        case QAbstractSocket::ConnectedState:

            m_lock_actualOpen.lock();
                m_actualOpen = true;
            m_lock_actualOpen.unlock();

            m_lock_shouldOpen.lock();
                m_shouldOpen = false;
            m_lock_shouldOpen.unlock();
            break;
        case QAbstractSocket::UnconnectedState:
            m_lock_actualOpen.lock();
                m_actualOpen = false;
            m_lock_actualOpen.unlock();

            m_lock_shouldOpen.lock();
                m_shouldOpen = true;
            m_lock_shouldOpen.unlock();
            break;
        case QAbstractSocket::ConnectingState:
        case QAbstractSocket::ClosingState:
            m_lock_actualOpen.lock();
                m_actualOpen = false;
            m_lock_actualOpen.unlock();

            m_lock_shouldOpen.lock();
                m_shouldOpen = false;
            m_lock_shouldOpen.unlock();
            break;
        default:break;
    }
    //qDebug()<<__FILE__ <<":" << __func__ <<"; Socket Status: " << socket->state()<<endl;
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    ConnectStatus talkerState;
    talkerState.ip = ip;
    //talkerState.state = socket->state();
    m_lock_Network.lock();
    talkerState.network_state = this->m_Network_Ok;
    m_lock_Network.unlock();
    emit reportConnectionState(talkerState);
}

void CloudTalker::connected()   //与Socket连接相关的 槽
{
    //qDebug()<<__FILE__ <<":" << __func__ <<"; Socket Status: " << socket->state()<<endl;
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    //state = QAbstractSocket::ConnectedState;
    ConnectStatus talkerState;
    talkerState.ip = ip;
    //talkerState.state = socket->state();
    m_lock_Network.lock();
        talkerState.network_state = true;
    m_lock_Network.unlock();

    m_lock_Network.lock();
        m_Network_Ok_last = m_Network_Ok;
        m_Network_Ok = true;
    m_lock_Network.unlock();

    emit reportConnectionState(talkerState);
}

void CloudTalker::disconnected()   //与Socket连接相关的 槽
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    //state = QAbstractSocket::UnconnectedState;
    ConnectStatus talkerState;
    talkerState.ip = ip;
    //talkerState.state = socket->state();

    m_lock_Network.lock();
        talkerState.network_state = false;
    m_lock_Network.unlock();

    m_lock_Network.lock();
        m_Network_Ok_last = m_Network_Ok;
        m_Network_Ok = false;
    m_lock_Network.unlock();

    emit reportConnectionState(talkerState);
}

void CloudTalker::heartBeat()
{
    //qDebug()<<__FILE__<<", " <<__FUNCTION__<<QThread::currentThreadId()<<endl;
    QString MN = localService->sysConfig.SysDic.getMN();
    QString ST = localService->sysConfig.SysDic.getST();
    m_lock_Network.lock();
      bool r = m_Network_Ok && m_Network_Ok_last;
    m_lock_Network.unlock();
    if(r){
        //定时向服务端发送心跳包
        QString dt = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");

        QString qnMsg = QString("QN=%1;ST=%2;CN=9015;PW=123456;MN=%3;Flag=4;CP=&&&&").arg(dt).arg(ST).arg(MN);
        bool isOK = false;
        QString msg = packingTools.CloudCmdBuilder(qnMsg, &isOK);
        if(socket->state()==QAbstractSocket::ConnectedState){
            socket->flush();
            socket->write(msg.toLatin1());
            socket->flush();
            //qDebug()<<__FILE__<<", " <<__FUNCTION__<< ", "<<qnMsg<<endl;
        }
    }
}

void CloudTalker::checkServerStatusSlot()
{
 //   qDebug()<<__FILE__<<", " <<__FUNCTION__<<QThread::currentThreadId()<<endl;

    bool pingOk = pingServer();
    bool socketOk = socket->state()==QAbstractSocket::ConnectedState?true:false;
    m_lock_Network.lock();
        m_Network_Ok_last = m_Network_Ok;
        m_Network_Ok = pingOk && socketOk;
        //m_Network_Ok = pingOk;
    m_lock_Network.unlock();

}

void CloudTalker::periodicalReportServerStatusSlot()
{
 //   qDebug()<<__FILE__<<", " <<__FUNCTION__<<QThread::currentThreadId()<<endl;
    ConnectStatus talkerState;
    talkerState.ip = ip;
    //talkerState.state = socket->state();
    m_lock_Network.lock();
        talkerState.network_state = this->m_Network_Ok;
    m_lock_Network.unlock();
    emit reportConnectionState(talkerState);
}

void CloudTalker::socketReConnnectSlot()
{
//    qDebug()<<__FILE__<<", " <<__FUNCTION__<<":: time: "<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")<<endl;
//    qDebug()<<__FILE__<<", " <<__FUNCTION__<<":: m_Network_Ok_last="<<m_Network_Ok_last<<",m_Network_Ok="<<m_Network_Ok<<endl;
//qDebug()<<__FILE__<<", " <<__FUNCTION__<<", "<<socket->state()<<endl;


    //    bool shouldOpen = false;
//    bool actualOpen = false;
//    {
//        QMutexLocker locker(&m_lock_shouldOpen);
//        shouldOpen = m_shouldOpen;
//    }
//    {
//        QMutexLocker locker(&m_lock_actualOpen);
//        actualOpen = m_actualOpen;
//    }
    bool pingOk = pingServer();
    if(!pingOk){
        this->socket->disconnectFromHost();
        //bool r = this->socket->waitForDisconnected(30*1000);
        //qDebug()<<__FILE__<<", " <<__FUNCTION__<<", waitForDisconnected() result is: "<<r <<", disconnectFromHost() result is: "<<socket->state()<<endl;
        //qDebug()<<__FILE__<<", " <<__FUNCTION__ <<", disconnectFromHost() result is: "<<socket->state()<<endl;
    }
    if(pingOk && socket->state()==QAbstractSocket::UnconnectedState)
    //if(m_Network_Ok_last==false && m_Network_Ok ==false )
    {
        this->socket->connectToHost(QHostAddress(ip), port);
        QMutexLocker locker(&m_lock_Network);
        if(this->socket->waitForConnected(1000))
            qDebug()<<__FILE__<<", " <<__FUNCTION__<<", Reconnected !"<<endl;
        m_Network_Ok = true;
    }
//    else{
//        if(shouldOpen){   //应该打开
//            if(actualOpen==false && socket->state() == QTcpSocket::UnconnectedState){
//                this->socket->connectToHost(QHostAddress(ip), port);
//            }
//        }
//        else if(shouldOpen==false){  //应该关闭
//            if(actualOpen || socket->state() != QTcpSocket::UnconnectedState){
//                this->socket->disconnectFromHost();
//            }
//        }
//    }
}

void CloudTalker::deleteDeadWorkerSlot()
{
    emit deleteDeadWorker(this->Duration);
}

bool CloudTalker::isNetworkCardEnabled()//检查是否有启用了网卡（但不保证能够上网）
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool isConnected = false;

    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if ( iface.flags().testFlag(QNetworkInterface::IsUp)
             && iface.flags().testFlag(QNetworkInterface::IsRunning)
             && !iface.flags().testFlag(QNetworkInterface::IsLoopBack)
             )
        {    
            // this loop is important
            for (int j=0; j<iface.addressEntries().count(); j++)
            {
                // we have an interface that is up, and has an ip address
                // therefore the link is present

                // we will only enable this check on first positive,
                // all later results are incorrect
                if (isConnected == false)
                    isConnected = true;
            }
        }
    }
    return isConnected;
}

bool CloudTalker::pingServer()//检查是否能够ping 外网
{
    //QString pingCmd = QString("ping %1 -W 500 -c 2").arg(this->ip);
    SysDictionary sysDic;
    QList<QString> serverList =sysDic.getPING_SERVERS_List();

    Ping ping;
    bool ret = ping.CheckNetWork();
    //bool ret = ping.CheckNetWork(serverList);

    return ret;
}
void CloudTalker::startCloudTalker(){ // 根据冒号分解得到IP和Port

    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    //====================================================================================================
    if(socket!=nullptr || worker!=nullptr) return;

    socket = new QTcpSocket();

    worker = new Worker(ip, ""+port,PW,MN, this);   //为哪个云端ip服务的Worker;
    //connect(socket, &QTcpSocket::connected, this, &CloudTalker::connected);
    //connect(socket, &QTcpSocket::disconnected, this, &CloudTalker::disconnected);

    //connect(socket, &QTcpSocket::stateChanged, this, &CloudTalker::handleStateChanged);
    //connect(socket, &QTcpSocket::readyRead, this, &CloudTalker::handleReadyRead,Qt::ConnectionType::DirectConnection);   //接收socket数据报

    connect(socket, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));

    connect(this, &CloudTalker::deleteDeadWorker, worker, &Worker::deleteDeadWorker );  //删除死Worker
    connect(this, &CloudTalker::taskDispatcher, worker, &Worker::doWork );  //分配任务干活
    connect(worker, &Worker::toCloud, this, &CloudTalker::handleToCloud);     //接收Worker的toCloud信号，
    //=====================================================================================================

    {
        QMutexLocker locker(&m_lock_shouldOpen);
        m_shouldOpen = true;
    }
    socket->connectToHost(QHostAddress(ip), port);



}
void CloudTalker::stopCloudTalker(){
    QMutexLocker locker(&m_lock_shouldOpen);
    m_shouldOpen = false;
    //socket.disconnectFromHost();   //由前面构造函数中的定时器来执行断开连接操作；
}
void CloudTalker::handleToCloud(QString msg)   //接收内部发往云端的数据包  槽
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    //qDebug()<<__FILE__<<", "<<__func__<<",Connect Status: " << socket->state()<<"," <<msg<<endl;
   //检查socket连接是否保持
    if(socket->state()==QAbstractSocket::ConnectedState){
        socket->flush();
        socket->write(msg.toLatin1()); //socket->write()
//        int c = socket->bytesToWrite();
        socket->flush();
    }

}


