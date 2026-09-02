#include <QThread>

#include "SubWorkerBase.h"
#include "Worker.h"
#include "SubWorker/SubWorker2011.h"
#include "SubWorker/SubWorker2021.h"

#include "SubWorker/SubWorker1000.h"
#include "SubWorker/SubWorker1011.h"
#include "SubWorker/SubWorker1012.h"
#include "SubWorker/SubWorker1013.h"
#include "SubWorker/SubWorker1061.h"
#include "SubWorker/SubWorker1062.h"
#include "SubWorker/SubWorker1063.h"
#include "SubWorker/SubWorker1064.h"
#include "SubWorker/SubWorker1072.h"
#include "SubWorker/SubWorker2012.h"
#include "SubWorker/SubWorker2031.h"
#include "SubWorker/SubWorker2051.h"
#include "SubWorker/SubWorker2061.h"
#include "SubWorker/SubWorker2062.h"
#include "SubWorker/SubWorker2081.h"
#include "SubWorker/SubWorker3011.h"
#include "SubWorker/SubWorker3012.h"
#include "SubWorker/SubWorker3013.h"
#include "SubWorker/SubWorker3014.h"
#include "SubWorker/SubWorker3015.h"

#include "SubWorker/SubWorker3016.h"
#include "SubWorker/SubWorker3017.h"
#include "SubWorker/SubWorker3018.h"
#include "SubWorker/SubWorker3019.h"
#include "SubWorker/SubWorker3020.h"
#include "SubWorker/SubWorker3021.h"
#include "SubWorker/SubWorker3044.h"
#include "SubWorker/SubWorker3080.h"
#include "SubWorker/SubWorker3085.h"
#include "SubWorker/SubWorker3086.h"

#include "./plc.h"
extern PLC plc;
//Worker::Worker(QString serverIP,QObject *parent) : QObject(parent),ip(serverIP),sqlTools(new SqliteTools(ip))
Worker::Worker(QString serverIP, QString serverPort, QString pw, QString mn,QObject *parent) : QObject(parent),sqlTools(new SqliteTools(serverIP+":"+serverPort)),IP(serverIP), PORT(serverPort), PW(pw),MN(mn)
//Worker::Worker(QString serverIP, QString pw, QString mn,QObject *parent) : QObject(parent),IP(serverIP),PW(pw),MN(mn)
{

}

Worker::~Worker()
{
    //qDebug()<<__FILE__<<", "<<__FUNCTION__<<endl;
    if(sqlTools!=nullptr){
        //qDebug()<<__FILE__<<", "<<__FUNCTION__<<", sqlTools is not nullptr."<<endl;
        delete sqlTools;
        sqlTools = nullptr;
    }
}
void Worker::doWork(Task task)
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    SubWorkerBase *swb=NULL;

    //qDebug()<<__FILE__<<":"<<__func__<<endl;
    DataPacketData data = task.dataPacket.data;

    //qDebug()<<"IP="<<this->IP<<endl;

    if(sqlTools->isNewDialog(data.QN, this->IP))
    {

         switch(data.CN.toInt()){
             case 1000:
                swb = new SubWorker1000(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1011:
                swb = new SubWorker1011(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1012:
                swb = new SubWorker1012(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1013:
                swb = new SubWorker1013(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1061:
                swb = new SubWorker1061(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1062:
                swb = new SubWorker1062(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1063:
                swb = new SubWorker1063(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1064:
                swb = new SubWorker1064(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 1072:
                swb = new SubWorker1072(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 2011:
                 //1.判断内存中是否有，如有，找到相关的对象，
                 //生成新的(两种情况：1.云端新的请求， taskSource是CLOUD_REQUET；2. 主动上报，taskSource是SELF_REPORT)

                //swb = new SubWorker2011(this->ip);  //应该是具体的子类对象
                swb = new SubWorker2011(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 2012:
                 //1.判断内存中是否有，如有，找到相关的对象，
                 //生成新的(两种情况：1.云端新的请求， taskSource是CLOUD_REQUET；2. 主动上报，taskSource是SELF_REPORT)

                //swb = new SubWorker2011(this->ip);  //应该是具体的子类对象
                swb = new SubWorker2012(this->IP, this->PW, this->MN, sqlTools);  //应该是具体的子类对象
                break;
             case 2021:
                 swb = new SubWorker2021(this->IP, this->PW, this->MN, sqlTools);
                 break;
             case 2031:
                 swb = new SubWorker2031(this->IP, this->PW, this->MN, sqlTools);
                 break;
             case 2041:
                 //swb = new SubWorker2041(this->IP, this->PW, this->MN, sqlTools);
                 break;
                case 2051:
                 swb = new SubWorker2051(this->IP, this->PW, this->MN, sqlTools);
                 break;
             case 2061:
                 swb = new SubWorker2061(this->IP, this->PW, this->MN, sqlTools);
                 break;
             case 2081:
                 swb = new SubWorker2081(this->IP, this->PW, this->MN, sqlTools);
                 break;
//         case 3011:
//             swb = new SubWorker3011(this->IP, this->PW, this->MN, sqlTools);
//             break;
         case 3012:
             swb = new SubWorker3012(this->IP, this->PW, this->MN, sqlTools);
             break;
//         case 3013:
//             swb = new SubWorker3013(this->IP, this->PW, this->MN, sqlTools);
//             break;
//         case 3014:
//             swb = new SubWorker3014(this->IP, this->PW, this->MN, sqlTools);
//             break;
         case 3015:
             swb = new SubWorker3015(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3015*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3016:
             swb = new SubWorker3016(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3016*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3017:
             swb = new SubWorker3017(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3017*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3018:
             swb = new SubWorker3018(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3018*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3019:
             swb = new SubWorker3019(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3019*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;


         case 3020:
             swb = new SubWorker3020(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3020*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3021:
             swb = new SubWorker3021(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3021*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3044:
             swb = new SubWorker3044(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3044*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3080:
             swb = new SubWorker3080(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3080*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3085:
             swb = new SubWorker3085(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3085*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         case 3086:
             swb = new SubWorker3086(this->IP, this->PW, this->MN, sqlTools);
             connect((SubWorker3086*)swb, &SubWorkerBase::toPLC, &plc, &PLC::requestHandler);
             break;
         default:
                 break;
         }
    }
    else{
        swb = (SubWorkerBase *)(sqlTools->findObjectByQN(data.QN, this->IP));
        //swb->disconnect(this);
        //这个时候还需要再次确认过来的task中的taskSource是否与内存表中的记录一致，以内存表中的taskSource为准

        int taskSource = sqlTools->findTaskSourceByQN(data.QN, this->IP);
        task.taskSource = (TaskDirection)taskSource;
    }

    if (swb!=NULL)
    {
        connect(swb, SIGNAL(toCloud(QString)), this, SLOT(handleToCloud(QString)),Qt::UniqueConnection);
        connect(swb, SIGNAL(isDone(QString)), this, SLOT(destroy(QString)),Qt::UniqueConnection);
        //connect(this, &Worker::dispatchTask, swb, &SubWorkerBase::doWork);
        connect(this, SIGNAL(dispatchTask(Task)), swb, SLOT(doWork(Task)),Qt::UniqueConnection);
        //emit dispatchTask(task);
        swb->doWork(task);
        usleep(100);
    }
}
void Worker::handleToCloud(QString msg)   //接收SubWorker信号  槽
{
    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    //qDebug()<<__FILE__<<":"<<__func__<<","<<msg<<endl;
    emit toCloud(msg);   //向上发送信号给CloudTalker
    usleep(100);
}

void Worker::destroy(QString QN)
{

    //qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;
    //qDebug()<<__FILE__<<":"<<__func__<<", QN = " <<QN <<endl;
    sqlTools->deleteDialogRecord(QN, this->IP);   //删除内存记录后，同时删除对象
}

void Worker::deleteDeadWorker(int afterMinutes)
{
    //qDebug()<<__FILE__<<", "<<__func__<<", afterMinutes="<<afterMinutes<<endl;
    sqlTools->deleteDeadWorker(afterMinutes);
}
