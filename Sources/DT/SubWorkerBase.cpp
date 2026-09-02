//#include "PackingTools.h"   //整合的时候必须删除，否则就循环包含
#include "SubWorkerBase.h"


#include "../../Headers/BLL/BLocalService.h"

extern BLocalService *localService;
SubWorkerBase::SubWorkerBase()
{
    this->timer.setInterval(this->TimeOut*1000);
    connect(&(this->timer), SIGNAL(&QTimer::timeout), this, SLOT(timerMethod));
}
//SubWorkerBase::SubWorkerBase(QString ip , SqliteTools sqlTools, QObject *parent) : QObject(parent),sqlTools(&sqlTools)
SubWorkerBase::SubWorkerBase(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent) : QObject(parent),sqlTools(sqlTools),PW(pw),MN(mn)
{
    this->IP = ip;
//    this->TimeOut = localService->sysConfig.SysDic.getTIMEOUT();
//    this->RepeatAllowable = localService->sysConfig.SysDic.getREPEAT();
    this->timer.setInterval(this->TimeOut*1000);
    connect(&(this->timer), &QTimer::timeout, this, &SubWorkerBase::timerMethod);
}
//SubWorkerBase::SubWorkerBase(const SubWorkerBase &swb):sqlTools(swb.sqlTools)
SubWorkerBase::SubWorkerBase(const SubWorkerBase &swb)
{
    this->IP = swb.IP;
    this->PNO = swb.PNO;
    this->QN1 = swb.QN1;
    this->QN2 = swb.QN2;
    this->Flag = swb.Flag;
    this->PNUM = swb.PNUM;
    this->TimeOut = swb.TimeOut;
    //this->sqlTools = swb.sqlTools;
    this->LatestSendCmd = swb.LatestSendCmd;

}
SubWorkerBase &SubWorkerBase::operator=(const SubWorkerBase &swb)
{
    this->IP = swb.IP;
    this->PNO = swb.PNO;
    this->QN1 = swb.QN1;
    this->QN2 = swb.QN2;
    this->Flag = swb.Flag;
    this->PNUM = swb.PNUM;
    this->TimeOut = swb.TimeOut;
    this->LatestSendCmd = swb.LatestSendCmd;
}

SubWorkerBase::~SubWorkerBase()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}
void SubWorkerBase::startTimer()
{
    this->timer.start();
}

void SubWorkerBase::stopTimer()
{
    this->timer.stop();
}

void SubWorkerBase::timerMethod()  //计时器到时，开始检查是否搬砖超时 >TimeOut
{
    this->CurrentRepeatNo++;
    if(this->CurrentRepeatNo>this->RepeatAllowable){
        QString dt = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
        //localService->writeToUploadList(dt,this->LatestSendCmd);
        emit isDone(this->QN1);
        this->stopTimer();
    }
    else{// Send Messsage Once Again !
        QString msg = this->LatestSendCmd;
        PackingTools pt;
        bool isOK = false;
        QString s = pt.CloudCmdBuilder(msg, &isOK);
        if(isOK){
            qDebug()<<__FILE__<<" "<<__func__<<": "<<"CurrentRepeatNo= "<<CurrentRepeatNo<<", msg="<<s<<endl;
            emit toCloud(s);
        }

    }
}
void SubWorkerBase::doWork(Task task)
{
//    QString msg = buildCloudCmd(task);
//    emit toCloud(msg);
    //    emit isDone("20211022183050223");
}


QString SubWorkerBase::buildCloudCmd(Task &task,bool &shouldDone)
{
//    QString msg = "##0091QN=20211022183050223;ST=32;CN=2011;PW=123456;MN=88880082086206;Flag=5;CP=&&RtdInterval=30&&5de8\r\n";
//    return msg;
    return "";
}

bool SubWorkerBase::validateCmd(QString cmd)
{
    return true;
}
