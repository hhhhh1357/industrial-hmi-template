#include "SubWorker3017.h"


//3017 提取采样时间周期(上位机发起）
#include <QStringBuilder>

#include <Headers/Model/MFactor.h>

#include <Sources/DT/plc.h>

#include <Headers/BLL/BLocalService.h>

extern PLC plc;
extern BLocalService * localService;
SubWorker3017::SubWorker3017(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}

SubWorker3017::~SubWorker3017()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker3017::doWork(Task task)
{
    PackingTools pt;
    //================================================================================================================================================
    //先要验证上位机请求命令是否有效
    bool isValidCmd = true;

    bool isOK = false;
    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = task.dataPacket.data.QN;
        QString ST = task.dataPacket.data.ST;
        QString CN = task.dataPacket.data.CN;
        QString PW =task.dataPacket.data.PW;
        QString MN = task.dataPacket.data.MN;

        QString DataTime = dpd.CP.DataTime;

        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){ //来自云端的新的命令，需要一个回复
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 返回请求应答
            //标准格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");

            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);

            //2.向PLC请求“采样时间周期”
            MFactor mf;

            localService->findFactor(dpd.PolId, mf);//找到检测污染因子所在的端口信息

            task.req.action = PLCAction::GET_Sampling_Period;
            task.req.stationNo = mf.stationAddr;
            task.req.plcAddr = mf.registerAddr;
            QString portName = mf.dataPortName;
            QStringList list = portName.split('-');

            if(list[0]=="RS232")
                task.req.port = (URT_PORT)(list[1].toInt()-1);
            else  //RS485
                task.req.port = (URT_PORT)(list[1].toInt()+4);

            //connect(this, SIGNAL(toPLC(Task)), &plc, SLOT(requestHandler(Task)));
            emit toPLC(task);

        }
        else if(!isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){  //处理PLC返回的结果，向云端报送
            //发送“提取采样时间周期”响应
            //格式：QN=20160801085857223;ST=32;CN=3017;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&PolId=w01018;CstartTime=000000;CTime=2&&

            QString PolId = task.dataPacket.data.PolId;

            QString CTime = task.res.resultMap.value("CTime");
            QString dpdDataString = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=4;CP=&&PolId=%6;CstartTime=000000;CTime=%7&&").arg(QN).arg(ST).arg(CN).arg(PW).arg(MN).arg(PolId).arg(CTime);
            task.dataPacket.data.SelfReportCmd = dpdDataString;
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);

            this->LatestSendCmd = task.dataPacket.data.SelfReportCmd;

            emit toCloud(msg);  //

            //3. 返回执行结果
            //标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
            dpdDataString = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=4;CP=&&ExeRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            //4. 删除记录
            emit isDone(QN);
        }
    }
    else{

    }
    //================================================================================================================================================


}

QString SubWorker3017::buildCloudCmd(Task &task, bool &shouldDone)
{
  return "";
}

