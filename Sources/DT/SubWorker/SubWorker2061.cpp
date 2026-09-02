//2061 取污染物小时数据（上位机发起）&& 上传污染物小时数据（下位机发起）

#include "SubWorker2061.h"

#include <Headers/BLL/BLocalService.h>


extern BLocalService * localService;

SubWorker2061::SubWorker2061(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}

SubWorker2061::~SubWorker2061()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker2061::doWork(Task task)
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
        QString BeginTime = dpd.CP.BeginTime;
        QString EndTime = dpd.CP.EndTime;

        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){ //来自云端的新的命令，需要一个回复
            this->QN1 = QN;
            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
            //1. 返回请求应答 标准格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");
            //QString msg = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            //2. 发送污染物数据（分包）
            QList<QString> cmdString;
            bool r = localService->getLocalHourData(BeginTime,EndTime,cmdString);
            for(int i = 0; i<cmdString.size();i++){
                this->PNO++;
                QString qnMsg = cmdString.at(i);
                QString msg = pt.CloudCmdBuilder(qnMsg,&isOK);
                emit toCloud(msg);
                //QThread::msleep(200);
            }
            //3. 返回执行结果，标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
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
//void SubWorker2061::doWork(Task task)
//{
//    PackingTools pt;
//    //================================================================================================================================================
//    //先要验证上位机请求命令是否有效
//    bool isValidCmd = true;

//    bool isOK = false;

//    if(isValidCmd){
//        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
//        DataPacketData dpd = task.dataPacket.data;
//        QString QN = dpd.QN;
//        QString CN = dpd.CN;
//        QString ST = dpd.ST;
//        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
//        if(isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){ //表示自云端发起的新一轮会话Session，需要一个回复
//            //1. 新的一轮会话Session
//            this->ReplySeqNo++;

//            //1.1-0  返回请求应答 标准格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
//            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=4;CP=&&QnRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
//            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
//            emit toCloud(msg);

//        }
//        if(isNewDialog){  //如果是新的一轮会话Session的开始，需要提取所有待发送的数据，并记录本次会话QN就是新一轮会话Session的QN
//            //1.1 查询所有数据放入packList中
//            BLocalService ls;
//            QString beginTime = dpd.CP.BeginTime;
//            QString endTime = dpd.CP.EndTime;
//            //QMap<QString, QString> dataList;

//            QList<QString> cmdStringList;
//            ls.getLocalHourData(beginTime, endTime, cmdStringList);


//            //待商量


//            /*QMapIterator<QString,QString> iter(dataList);
//            int count = 0;
//            while(iter.hasNext()){
//                iter.next();
//                count++;
//                this->packList.insert(QString::number(count), iter.value());  //
//            }
//            this->PNO = 0;
//            this->PNUM = packList.size();
//            this->QN1 = QN;

//            //1.2  保存本轮会话Session入内存数据库
//            sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);*/
//        }
//        else{
//            //2. 表示是旧的一轮会话Session中的一个对话阶段
////            //2.1 产生对话阶段QN，
////            QDateTime dateTime = QDateTime::currentDateTime();
////            //2.2 在代表一轮会话Session的对象中，修改用于描述对话阶段的QN
////            this->QN2 = dateTime.toString("yyyyMMddhhmmsszzz");
//            this->QN2 = QN;

//            //2.3 更新会话Session状态表中对应记录的QN2列
//            sqlTools->continueDialogRecord(QN1, QN2, this->IP);
//        }
//        //3. 开始数据提交并接收云端回复的一问一答会话
//        //3.1 从packList中取数据，构造发向云端的命令串的QN部分
//        this->PNO++;
//        QString str = packList[QString::number(PNO)];
//        QString qnMsg = "";
//        if(this->PNO<=this->packList.size() && str == ""){
//            this->ReplySeqNo++;
//            qnMsg = QString("QN=%1;ST=2%;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&%7&&").arg(this->QN2).arg(this->ST).arg(this->CN).arg(this->PW).arg(this->MN).arg("7").arg(packList[QString::number(this->PNO)]);
//            //3.2 将命令串的QN部分，打包成发送到云端的命令串（格式为：##+数据段长度+QN部分数据段+CRC校验+包尾）
//            QString msg = pt.CloudCmdBuilder(dpd, &isOK);
//            this->LatestSendCmd = task.dataPacket.data.SelfReportCmd;
//            this->startTimer();
//            emit toCloud(msg);
//        }
//        else{ //表示没有数据需要发送，这个时候就应该发送一个表示对话结束的信息串；同时停止定时重发功能，并删除内存对象和内存表记录，彻底完成本轮对话Session(对应于QN1)
//            this->ReplySeqNo++;
//            //返回执行结果,标准格式： QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&
//            qnMsg = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=4;CP=&&ExeRtn=1&&").arg(this->QN1).arg(this->PW).arg(this->MN); //返回执行结果，结束对话
//            QString msg = pt.CloudCmdBuilder(dpd, &isOK);
//            emit toCloud(msg);
//            this->stopTimer();
//            emit isDone(this->QN1);
//        }

//    }
//    else{//表示接收到的是无效命令，暂时作如下处理（尚待商量 .......）


//    }

////    QString msg = "##0091QN=20211022183050223;ST=32;CN=2011;PW=123456;MN=88880082086206;Flag=5;CP=&&RtdInterval=30&&5de8\r\n";
////    return msg;
//}

QString SubWorker2061::buildCloudCmd(Task &task, bool &shouldDone)
{
    bool isValidCmd = true;

    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
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

