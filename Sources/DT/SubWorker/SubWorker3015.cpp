//3015 超标留样(上位机发起）
#include "SubWorker3015.h"


#include <QStringBuilder>
#include <Headers/BLL/BLocalService.h>

#include "../plc.h"
#include "../SamplerState.h"
#include <Headers/Model/MFactor.h>

extern PLC plc;
extern BLocalService * localService;
SubWorker3015::SubWorker3015(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}

SubWorker3015::~SubWorker3015()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker3015::doWork(Task task)
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

        QString DataTime = dpd.CP.DataTime;

        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){ //来自云端的新的命令，需要一个回复
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 返回请求应答
            //标准格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");
            //QString msg = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);

            //执行超标留样...
            MFactor mf;
            localService->findFactor("wqs", mf);
            task.req.action = PLCAction::CMD_SAMPLING_3015;
            task.req.stationNo = mf.stationAddr;
            task.req.plcAddr = mf.registerAddr;
            QString portName = mf.dataPortName;
            QStringList list = portName.split('-');
            int p;
            if(list[0]=="RS232")
                task.req.port = (URT_PORT)(list[1].toInt()-1);
            else  //RS485
                task.req.port = (URT_PORT)(list[1].toInt()+4);

            //connect(this, SIGNAL(toPLC(Task)), &plc, SLOT(requestHandler(Task)));
            emit toPLC(task);


        }
        else if(!isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){  //处理PLC返回的结果，向云端报送
            //2.发送“超标留样”响应
            //标准格式：QN=20160801085857223;ST=32;CN=3015;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&& DataTime=20160801085857;VaseNo=1 &&
            QString VaseNo = task.dataPacket.data.CP.VaseNo;
            QString DataTime = QDateTime::currentDateTime().toString("YYYYMMddhhmmss");
            QString dpdDataString = QString("QN=%1;ST=%2;CN=3015;PW=%3;MN=%4;Flag=4;CP=&&DataTime=%5;VaseNo=%6&&").arg(this->QN1).arg(ST).arg(PW).arg(MN).arg(DataTime).arg(VaseNo);
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);

            //3. 返回执行结果
            //标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
            dpdDataString = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=4;CP=&&ExeRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            emit isDone(QN1);
        }

    }
    else{

    }
    //================================================================================================================================================


}

QString SubWorker3015::buildCloudCmd(Task &task, bool &shouldDone)
{
  return "";
}
