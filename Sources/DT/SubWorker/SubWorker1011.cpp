//1011 提取现场机时间（上位机发起）  && 上传现场机时间（下位机发起）

#include "SubWorker1011.h"

#include <Headers/BLL/BLocalService.h>

extern BLocalService * localService;
SubWorker1011::SubWorker1011(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw,  mn,sqlTools, parent)
{

}
SubWorker1011::~SubWorker1011()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker1011::doWork(Task task)
{
    bool isValidCmd = true;
    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString polId= dpd.PolId;

        this->QN1 = QN;
        sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
        //1. 返回请求应答(标准中的回复命令格式)
        //QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&

        QString msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&QnRtn=%7&&").arg(QN).arg("91").arg("9011").arg(PW).arg(MN).arg("4").arg("1");
        emit toCloud(msg);
        //2. 执行。调用或发消息（根据polId获取数采仪的设置，获得数采仪时间）
        //拼接PLC读取时间命令
        QString plcCmd = "";
        //。。。。。
        QString SystemTime="";   //来自数采仪

        if (polId=="")  //取数采仪时间
        {
            SystemTime = localService->getLocalDatetime();
            msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&SystemTime=%7&&").arg(QN).arg(ST).arg(CN).arg(PW).arg(MN).arg("4").arg(SystemTime);
        }
        else   //取 PLC 时间
        {
            //QN=20160801085857223;ST=32;CN=1011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&& PolId=w01018;SystemTime=20160801085857&&  (标准中的回复命令格式)
            msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&PolId=%7;SystemTime=%8&&").arg(QN).arg(ST).arg(CN).arg(PW).arg(MN).arg("4").arg(polId).arg(SystemTime);
        }

        bool isOK=false;
        PackingTools pt;
        msg = pt.CloudCmdBuilder(msg, &isOK);
        emit toCloud(msg);
        //3. 返回执行结果
        //QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&   (标准中的回复命令格式)
        msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&ExeRtn=%7&&").arg(QN).arg("91").arg("9012").arg(PW).arg(MN).arg("4").arg("1");
        emit toCloud(msg);
        emit isDone(this->QN1);
    }
}

QString SubWorker1011::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";

}
