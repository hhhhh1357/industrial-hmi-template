#include "SubWorker3019.h"

//3019 提取设备唯一标识(上位机发起） && 上传设备唯一标识(下位机发起）
#include <QStringBuilder>

#include <Headers/Model/MFactor.h>

#include <Sources/DT/plc.h>

#include <Headers/BLL/BLocalService.h>

extern PLC plc;
extern BLocalService * localService;

SubWorker3019::SubWorker3019(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}

SubWorker3019::~SubWorker3019()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker3019::doWork(Task task)
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
        QString PW = dpd.PW;
        QString MN = dpd.MN;
        QString PolId = dpd.PolId;
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

            //2.发送“提取设备唯一标识”响应。格式：QN=20160801085857223;ST=32;CN=3019;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&PolId=w01018;w01018-SN=010000A8900016F000169DC1&&
            //.....
            MFactor mf;

            localService->findFactor(dpd.PolId, mf);//找到检测污染因子所在的端口信息

            task.req.action = PLCAction::GET_DEV_FLAG;
            task.req.stationNo = mf.stationAddr;
            task.req.plcAddr = mf.registerAddr;
            QString portName = mf.dataPortName;
            if (mf.plcDataType=="H1234")
                task.req.dataAlign = DataAlign::DA1234;//(DataAlign)1;
            else if (mf.plcDataType=="H4321")
                task.req.dataAlign = DataAlign::DA4321;//(DataAlign)0;
            else if (mf.plcDataType=="H3412")
                task.req.dataAlign = DataAlign::DA3412;//(DataAlign)2;
            else
                task.req.dataAlign = DataAlign::DA3412;//(DataAlign)2;

            QStringList list = portName.split('-');

            if(list[0]=="RS232")
                task.req.port = (URT_PORT)(list[1].toInt()-1);
            else  //RS485
                task.req.port = (URT_PORT)(list[1].toInt()+4);

            emit toPLC(task);
        }
        else if(!isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){
            //发送“提取设备唯一标识”响应
            //格式：QN=20160801085857223;ST=32;CN=3019;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&PolId=w01018;w01018-SN=010000A8900016F000169DC1&&
            QString SN = task.res.resultMap.value("devSerialNo");  //从PLC发过来的SN的值
            QString dpdDataString = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=4;CP=&&PolId=%6;%6-SN=%7&&").arg(QN).arg(ST).arg(CN).arg(PW).arg(MN).arg(PolId).arg(SN);
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
        else if(isNewDialog && taskSource == TaskDirection::SELF_REPORT){
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 上传设备唯一标识 标准格式：QN=20160801085857223;ST=32;CN=3019;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&&DataTime=20160801085857;PolId=w01018;w01018-SN=010000A8900016F000169DC1&&
            QString dpdDataString = "";
            //。。。。。。
            //QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
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

QString SubWorker3019::buildCloudCmd(Task &task, bool &shouldDone)
{
  return "";
}
