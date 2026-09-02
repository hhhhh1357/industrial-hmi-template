//1000 设置超时时间及重发次数（上位机发起）

#include "SubWorker1000.h"

#include <Headers/BLL/BLocalService.h>
extern BLocalService * localService;
SubWorker1000::SubWorker1000(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw,  mn,sqlTools, parent)
{

}
SubWorker1000::~SubWorker1000()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker1000::doWork(Task task)
{
    bool isValidCmd = true;
    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;

        QString ReCount = dpd.CP.ReCount;
        QString OverTime = dpd.CP.OverTime;

        this->QN1 = QN;
        sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
        //1. 返回请求应答
        //QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
        QString msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&QnRtn=%7&&").arg(QN).arg("91").arg("9011").arg(PW).arg(MN).arg("4").arg("1");
        bool isOK=false;
        PackingTools pt;
        msg = pt.CloudCmdBuilder(msg, &isOK);
        emit toCloud(msg);
        //2. 执行。调用或发消息（设置超时时间及重发次数的函数）

        localService->setTimeoutAndRepeat(OverTime,ReCount);

        //3. 返回执行结果
        //QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
        msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&ExeRtn=%7&&").arg(QN).arg("91").arg("9012").arg(PW).arg(MN).arg("4").arg("1");
        msg = pt.CloudCmdBuilder(msg, &isOK);
        emit toCloud(msg);
        emit isDone(this->QN1);
    }
}

QString SubWorker1000::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}
