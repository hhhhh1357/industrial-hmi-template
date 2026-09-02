//2021 取污染物日历史数据（上位机发起）&& 上传噪声声级日历史数据（下位机发起）

#include "SubWorker2031.h"

extern BLocalService * localService;

SubWorker2031::SubWorker2031(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip,pw,mn,sqlTools,parent)
{

}

SubWorker2031::~SubWorker2031()
{

}

void SubWorker2031::doWork(Task task){
    PackingTools pt;

    bool isValidCmd = true;

    bool isOK = false;
    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString BeginTime = dpd.CP.BeginTime;
        QString EndTime = dpd.CP.EndTime;
        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog&&taskSource == TaskDirection::CLOUD_REQUEST){
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);

            QList<QString> cmdString;
            bool r = localService->getLocalDayData(BeginTime,EndTime,cmdString);
            for(int i = 0; i<cmdString.size();i++){
                this->PNO++;
                QString qnMsg = cmdString.at(i);
                QString msg = pt.CloudCmdBuilder(qnMsg,&isOK);
                emit toCloud(msg);                
            }
            dpdDataString = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=4;CP=&&ExeRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            emit isDone(this->QN1);
        }
        else if(isNewDialog&& taskSource == TaskDirection::SELF_REPORT){
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            QString msg = pt.CloudCmdBuilder(dpd, &isOK);
            this->LatestSendCmd = task.dataPacket.data.SelfReportCmd;
            this->startTimer();
            emit toCloud(msg);
        }
        else{
            this->stopTimer();
            emit isDone(this->QN1);
        }
    }
    else{

    }
}

QString SubWorker2031::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}
