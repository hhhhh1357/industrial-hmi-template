//2021 取设备运行状态数据（上位机发起）&& 上传设备运行状态数据（下位机发起）

#include "SubWorker2021.h"

#include <QStringBuilder>
#include "../Worker.h"

#include <Headers/BLL/BLocalService.h>

extern BLocalService * localService;

//extern SqliteTools2 *sqlTools;
SubWorker2021::SubWorker2021(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
//SubWorker2021::SubWorker2011(QString ip, QObject *parent):SubWorkerBase(ip,  parent)
{

}
SubWorker2021::~SubWorker2021()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}


void SubWorker2021::doWork(Task task)
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
            //1. 返回请求应答 标准格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=1&&").arg(this->QN1).arg(PW).arg(MN);                        
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            //qDebug()<<__FILE__<<","<<__func__<<","<<msg<<endl;

            //2. 发送消息给DB或UI，启动上传设备运行状态数据
            localService->startUploadRealData();
            //3. 返回执行结果，标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
            dpdDataString = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=4;CP=&&ExeRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
        }
        else if(isNewDialog && taskSource == TaskDirection::SELF_REPORT){
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 上传设备运行状态数据 标准格式：QN=20160801085857223;ST=32;CN=2021;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&&DataTime=20160801085857; SB1-RS=1；SB2-RS=0..&&
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

QString SubWorker2021::buildCloudCmd(Task &task,bool &shouldDone)
{
/*
    PackingTools pt;
    if(this->ReplySeqNo==1){//第一次，把要报告的数据取出放入到QList中
        //一次性生成数据放入packList中
        //以下是模拟数据  ##0091QN=20211022183050223;ST=32;CN=2011;PW=123456;MN=88880082086206;Flag=5;CP=&&RtdInterval=30&&5de8\r\n
        PolData polData0;
        polData0.Avg="12";
        polData0.Max="19";
        polData0.Min="8";
        polData0.Rtd="10";
        polData0.Flag="N";
        polData0.Cou="120";
        polData0.PolId="w01018";

        packList.insert("1", polData0.toString());

    }
    if(task.taskSource==TaskDirection::SELF_REPORT){  //主动上报
        this->ReplySeqNo++;
        this->PNO++;
        QString str = packList[QString::number(PNO)];
        if(this->PNO>this->packList.size() || str == ""){  //表示没有数据需要发送，这个时候就应该发送一个表示对话结束的信息串
            QString msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&ExeRtn=1&&").arg(this->QN1).arg("91").arg("9012").arg(this->PW).arg(this->MN).arg("4"); //返回执行结果，结束对话
//                emit toCloud(msg);
//                usleep(100);
//                emit isDone(QN1);
            shouldDone = true;
            return msg;
        }
        else{
            PolData polData = polData.StrToPolData(str);
            CmdParas CP;
            CP.polDataList.append(polData);
            task.dataPacket.data.CP = CP;
            task.dataPacket.data.ST=this->ST;
            task.dataPacket.data.CN = this->CN;
            task.dataPacket.data.PW = this->PW;
            task.dataPacket.data.MN = this->MN;

            task.dataPacket.data.Flag = "7";
            task.dataPacket.data.PNO = QString::number(this->PNO);
            task.dataPacket.data.PNUM= this->PNUM;

            //产生新的QN，更新对话状态表中对应记录的QN2列
            QDateTime dateTime = QDateTime::currentDateTime();
            // 字符串格式化
            this->QN2 = dateTime.toString("yyyyMMddhhmmsszzz");
            sqlTools->continueDialogRecord(this->QN1, QN2, this->IP);
            task.dataPacket.data.QN = QN2;

            bool isOk = false;
            QString msg = pt.CloudCmdBuilder(task.dataPacket.data, &isOk);
            shouldDone = true;
            return msg;
        }
    }
    else if(task.taskSource == TaskDirection::CLOUD_REQUEST){
        if(this->ReplySeqNo++==1){

            DataPacketData dpd = task.dataPacket.data;
            QString msg ="QN=" + dpd.QN+";ST=91;CN=9011;PW=" + dpd.PW + ";MN=" + dpd.MN + ";Flag=4;CP=&&QnRtn=1&&";
            return msg;
        }
        else{
            this->PNO++;
            QString str = packList[QString::number(PNO)];
            if(this->PNO>this->packList.size() || str == ""){  //表示没有数据需要发送，这个时候就应该发送一个表示对话结束的信息串
                QString msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&ExeRtn=1&&").arg(this->QN1).arg("91").arg("9012").arg(this->PW).arg(this->MN).arg("4"); //返回执行结果，结束对话
//                emit toCloud(msg);
//                usleep(100);
//                emit isDone(QN1);
                shouldDone = true;
                return msg;
            }
            else{
                PolData polData = polData.StrToPolData(str);
                CmdParas CP;
                CP.polDataList.append(polData);
                task.dataPacket.data.CP = CP;
                task.dataPacket.data.ST=this->ST;
                task.dataPacket.data.CN = this->CN;
                task.dataPacket.data.PW = this->PW;
                task.dataPacket.data.MN = this->MN;

                task.dataPacket.data.Flag = "7";
                task.dataPacket.data.PNO = QString::number(this->PNO);
                task.dataPacket.data.PNUM= this->PNUM;

                //产生新的QN，更新对话状态表中对应记录的QN2列
                QDateTime dateTime = QDateTime::currentDateTime();
                // 字符串格式化
                this->QN2 = dateTime.toString("yyyyMMddhhmmsszzz");
                sqlTools->continueDialogRecord(this->QN1, QN2, this->IP);
                task.dataPacket.data.QN = QN2;

                bool isOk = false;
                QString msg = pt.CloudCmdBuilder(task.dataPacket.data, &isOk);
                shouldDone = true;
                return msg;
            }

        }
    }


    */
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

