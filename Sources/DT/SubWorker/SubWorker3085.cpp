//3085 启动单台仪表空白校准

#include "SubWorker3085.h"

#include <QStringBuilder>

#include <Headers/Model/MFactor.h>
#include <Headers/BLL/BLocalService.h>
extern BLocalService * localService;

SubWorker3085::SubWorker3085(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
//SubWorker3085::SubWorker3085(QString ip, QObject *parent):SubWorkerBase(ip,  parent)
{

}
SubWorker3085::~SubWorker3085()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}


void SubWorker3085::doWork(Task task)
{
    //qDebug()<<__FILE__<<","<<__func__<<","<<task.dataPacket.data.SelfReportCmd<<endl;
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
            //1. 返回请求应答 标准格式：QN=20101108135153914;ST=91;CN=9011;PW=123456;MN=A110000_0001;Flag=8;CP=&&QnRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=8;CP=&&QnRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            qDebug()<<__FILE__<<","<<__func__<<","<<msg<<endl;

            //2. 执行启动单台仪表空白校准命令。。。

            MFactor mf;
            bool r = localService->findFactor(dpd.PolId, mf);//找到检测污染因子所在的端口信息
            if(r){
                task.req.action = PLCAction::CMD_3085;
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

            //3. 返回执行结果，标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=8;CP=&&ExeRtn=1&&
            dpdDataString = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=8;CP=&&ExeRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            emit isDone(this->QN1);
        }

    }
    else{

    }
    //================================================================================================================================================


}

QString SubWorker3085::buildCloudCmd(Task &task,bool &shouldDone)
{

    return "";
    //    QString msg = "##0091QN=20211022183050223;ST=32;CN=2011;PW=123456;MN=88880082086206;Flag=5;CP=&&RtdInterval=30&&5de8\r\n";
    //    return msg;
}

