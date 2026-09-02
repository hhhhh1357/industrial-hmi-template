//2011 停止查看污染物实时数据（上位机发起）

#include "SubWorker2012.h"

#include <QStringBuilder>
#include "../Worker.h"

#include <Headers/BLL/BLocalService.h>
extern BLocalService * localService;


SubWorker2012::SubWorker2012(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}
SubWorker2012::~SubWorker2012()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}


void SubWorker2012::doWork(Task task)
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
        if(isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){ //来自云端的新的命令，需要一个回复
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 回应通知 标准格式：QN=20160801085857223;ST=91;CN=9013;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9013;PW=%2;MN=%3;Flag=4;CP=&&&&").arg(this->QN1).arg(PW).arg(MN);

            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            //2. 现场机执行“停止察看污染物实时数据”命令
            localService->stopUploadRealData();

            emit isDone(this->QN1);
        }
    }
    else{

    }
    //================================================================================================================================================


}

QString SubWorker2012::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}


