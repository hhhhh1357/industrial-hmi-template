#include "SubWorker2051.h"


#include <QStringBuilder>
#include "../Worker.h"

#include <Headers/BLL/BLocalService.h>

extern BLocalService * localService;


SubWorker2051::SubWorker2051(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}

SubWorker2051::~SubWorker2051()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker2051::doWork(Task task)
{
    PackingTools pt;
    //================================================================================================================================================
    //先要验证上位机请求命令是否有效
    bool isValidCmd = true;

    bool isOK = false;
    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString BeginTime = dpd.CP.BeginTime;
        QString EndTime = dpd.CP.EndTime;

        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){ //来自云端的新的命令，需要一个回复
            this->QN1 = QN;
            //1. 返回请求应答 标准格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");
            //QString msg = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            QThread::msleep(1000);
            //2. 现场机执行“取污染物分钟历史数据”请求命令（分包）
            QList<QString> cmdString;
            bool r = localService->getLocalMinuteData(BeginTime,EndTime,cmdString);
            //现场机依次上报请求时间段内污染物分钟数据
            for(int i = 0; i<cmdString.size();i++){
                this->PNO++;
                QString qnMsg = cmdString.at(i);
                QString msg = pt.CloudCmdBuilder(qnMsg,&isOK);
                emit toCloud(msg);
                //if(i>=4) break;
                QThread::msleep(500);
            }
            //3. 返回执行结果，标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
            dpdDataString = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=4;CP=&&ExeRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            emit isDone(this->QN1);
        }
        else if(isNewDialog && taskSource == TaskDirection::SELF_REPORT){
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 上传噪声声级实时数据 标准格式：QN=20160801085000001;ST=91;CN=9014;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&&&

            QString msg = pt.CloudCmdBuilder(dpd, &isOK);
            this->LatestSendCmd = task.dataPacket.data.SelfReportCmd;
            this->startTimer();
            emit toCloud(msg);
        }
        else
        {
            this->stopTimer();
            emit isDone(this->QN1);
        }

    }
    else{

    }
    //================================================================================================================================================


}

QString SubWorker2051::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}
