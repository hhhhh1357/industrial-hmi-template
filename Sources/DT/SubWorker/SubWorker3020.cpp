//提取现场机信息（日志）(上位机发起） && 上传现场机信息（日志）（下位机发起）

#include "SubWorker3020.h"


#include <QStringBuilder>
#include "../Worker.h"

#include "../PLCTaskHandler2.h"
#include <Headers/BLL/BLocalService.h>
#include "../plc.h"

extern BLocalService * localService;

extern PLC plc;
SubWorker3020::SubWorker3020(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}

SubWorker3020::~SubWorker3020()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker3020::doWork(Task task)
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
        QString PolId = dpd.PolId;
        QString InfoId = dpd.InfoId;

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
            //2.1 获得污染物因子对应的

            QString polId = dpd.PolId;
            QString infoId = dpd.InfoId;

            MFactor mf;
            bool r = localService->findFactor(polId,mf);
            //2.1 构造访问PLC的Task参数
            if(r){
                QString portName = mf.dataPortName;
                QStringList list = portName.split("-");
                URT_PORT port = URT_PORT::PORT0;
                if(list.at(0)=="RS232")
                    port = (URT_PORT)(list.at(1).toInt()-1);
                else if(list.at(0)=="RS485"){
                    port = (URT_PORT)(list.at(1).toInt()- 1 + 5);
                }
                task.req.port = port;
                task.req.action = PLCAction::GET_INSTRUMENT_INFO;
                task.req.cmdCode = mf.cmdCode;
                task.req.plcAddr = mf.registerAddr;
                task.req.stationNo = mf.stationAddr;

                task.taskSource = TaskDirection::CLOUD_REQUEST;  //以便全局变量plc向不同的调用者发isDone消息

                emit toPLC(task);
            }
            else{
                emit isDone(this->QN1);
            }

        }
        else if(!isNewDialog&&taskSource==TaskDirection::CLOUD_REQUEST){ //从PLC执行后的返回，向云端报送
            //2.发送“运行日志”信息
            //标准格式：QN=20160801085857334;ST=32;CN=3020;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&DataTime=20160801062035;PolId=w01018;i11001-Info=//清洗管路//&&
            DataTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
            QString dpdDataString = QString("QN=%1;ST=%2;CN=3020;PW=%3;MN=%4;Flag=4;CP=&&DataTime=%5;PolId=%6;%7-Info=//%8//&&").arg(this->QN1).arg(ST).arg(PW).arg(MN).arg(DataTime).arg(PolId).arg(InfoId).arg(task.res.resultMap.value(InfoId));

            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);

            emit toCloud(msg);

            //3. 返回执行结果
            //标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
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
            //this->startTimer();
            emit toCloud(msg);

        }        
        else
        {
            //this->stopTimer();
            emit isDone(this->QN1);
        }

    }
    else{

    }
    //================================================================================================================================================


}

QString SubWorker3020::buildCloudCmd(Task &task, bool &shouldDone)
{
  return "";
}
