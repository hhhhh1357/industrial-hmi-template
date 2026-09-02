//1013 现场机时间校准请求（下位机发起）

#include "SubWorker1013.h"


SubWorker1013::SubWorker1013(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw,  mn,sqlTools, parent)
{

}

SubWorker1013::~SubWorker1013()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker1013::doWork(Task task)
{
    //先要验证上位机请求命令是否有效
    bool isValidCmd = true;

    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString polId= dpd.PolId;    //在线监控（监测）仪器仪表对应污染物编码

        this->QN1 = QN;
        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog ){
            sqlTools->newDialogRecord(QN, (qlonglong)this, taskSource, this->IP);
            //1. 现场机发送时间校准请求命令(标准中的命令格式)
            //QN=20160801085857223;ST=32;CN=1013;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&&PolId=w01018&&
            QString msg;
            if (polId=="")
               msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&&&").arg(QN).arg(ST).arg(CN).arg(PW).arg(MN).arg("5");
            else
               msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&PolId=%7&&").arg(QN).arg(ST).arg(CN).arg(PW).arg(MN).arg("5").arg(polId);
            bool isOK=false;
            PackingTools pt;
            msg = pt.CloudCmdBuilder(msg, &isOK);
            this->LatestSendCmd = task.dataPacket.data.SelfReportCmd;
            this->startTimer();
            emit toCloud(msg);
        }
        else{//否则就是上位机的返回请求应答，标准格式是：QN=20160801085857223;ST=91;CN=9013;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&& &&
            this->stopTimer();
            emit isDone(this->QN1);
        }
    }
}

QString SubWorker1013::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}
