//3044 远程启动系统单次测试

#include "SubWorker3044.h"

#include <QStringBuilder>
#include <Headers/Model/MFactor.h>

#include <Sources/DT/plc.h>

#include <Headers/BLL/BLocalService.h>

extern PLC plc;
extern BLocalService * localService;
SubWorker3044::SubWorker3044(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}
SubWorker3044::~SubWorker3044()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}


void SubWorker3044::doWork(Task task)
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
            //qDebug()<<__FILE__<<","<<__func__<<","<<msg<<endl;

            //2. 执行远程启动系统单次测试命令。。。

            MFactor mf;
            bool r = localService->findFactor(dpd.PolId, mf);//找到检测污染因子所在的端口信息
            if(r){
                task.req.action = PLCAction::CMD_3044;
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

QString SubWorker3044::buildCloudCmd(Task &task,bool &shouldDone)
{

    //先要验证上位机请求命令是否有效
    bool isValidCmd = true;

    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString MN = dpd.MN;
        QString PW = dpd.PW;
        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){ //来自云端的新的命令，需要一个回复
            //1. 生成一条应答信息
            this->ReplySeqNo++;

            QString msg ="QN=" + dpd.QN+";ST=91;CN=9011;PW=" + dpd.PW + ";MN=" + dpd.MN + ";Flag=4;CP=&&QnRtn=1&&";
            shouldDone = false;
            return msg;
        }
        if(isNewDialog){//查询所有数据放入packList中
            //.....

            this->PNO = 0;
            this->PNUM = packList.size();
            this->QN1 = QN;
            this->ST = ST;
            this->MN = MN;
            this->PW = PW;
            this->CN = CN;

            sqlTools->newDialogRecord(QN, (qlonglong)this, taskSource, this->IP);
        }
        else{
            //产生新的QN，更新对话状态表中对应记录的QN2列
            QDateTime dateTime = QDateTime::currentDateTime();
            this->QN2 = dateTime.toString("yyyyMMddhhmmsszzz");

            sqlTools->continueDialogRecord(QN1, QN2, this->IP);
        }
        //从packList中取数据，构造命令串
        this->PNO++;
        QString str = packList[QString::number(PNO)];
        QString resultMsg = "";
        if(this->PNO>this->packList.size() || str == ""){  //表示没有数据需要发送，这个时候就应该发送一个表示对话结束的信息串
            this->ReplySeqNo++;
            resultMsg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&ExeRtn=1&&").arg(this->QN1).arg("91").arg("9012").arg(this->PW).arg(this->MN).arg("4"); //返回执行结果，结束对话
            shouldDone = true;
            return resultMsg;
        }
        else{
            resultMsg = QString("QN=%1;ST=2%;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&%7&&").arg(this->QN2).arg(this->ST).arg(this->CN).arg(this->PW).arg(this->MN).arg("7").arg(packList[QString::number(this->PNO)]);
            this->ReplySeqNo++;
            shouldDone = false;
            return resultMsg;
        }
    }
    else{//表示接收到的是无效命令，暂时作如下处理（尚待商量 .......）
        shouldDone = true;
        return "";
    }

//    QString msg = "##0091QN=20211022183050223;ST=32;CN=2011;PW=123456;MN=88880082086206;Flag=5;CP=&&RtdInterval=30&&5de8\r\n";
//    return msg;
}

