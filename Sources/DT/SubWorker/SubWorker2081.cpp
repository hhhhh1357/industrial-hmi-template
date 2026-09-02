#include "SubWorker2081.h"

#include <Headers/BLL/BLocalService.h>



SubWorker2081::SubWorker2081(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw,  mn,sqlTools, parent)
{

}

SubWorker2081::~SubWorker2081()
{

}

void SubWorker2081::doWork(Task task)
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
        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog && taskSource == TaskDirection::SELF_REPORT){
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 上传数采仪开机时间数据 标准格式：QN=20160801085857223;ST=32;CN=2081;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&&DataTime=20160801085857;RestartTime=20160801085624&&
            QString msg = pt.CloudCmdBuilder(dpd, &isOK);
            this->LatestSendCmd = task.dataPacket.data.SelfReportCmd;
            emit toCloud(msg);
        }
        else
        {
            emit isDone(this->QN1);
        }

    }
    else{

    }
}

QString SubWorker2081::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}

