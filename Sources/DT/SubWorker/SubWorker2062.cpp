//2062 提取监测指标核查数据（上位机发起）&& 上传监测指标核查数据（下位机发起）

#include "SubWorker2062.h"

#include <Headers/Model/MFactor.h>

#include <Sources/DT/plc.h>

#include <Headers/BLL/BLocalService.h>

extern PLC plc;
extern BLocalService * localService;
SubWorker2062::SubWorker2062(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw, mn,sqlTools, parent)
{

}

SubWorker2062::~SubWorker2062()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}
void SubWorker2062::doWork(Task task){
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
            //1. 返回请求应答 标准格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=A110000_0001;Flag=8;CP=&&QnRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9011;PW=%2;MN=%3;Flag=8;CP=&&QnRtn=%4&&").arg(this->QN1).arg(PW).arg(MN).arg("1");
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);

            //
            QList<MFactor> polIdList = localService->getAllFactor();
            this->PNUM = polIdList.size();

            //构建发送到PLC所需要的task参数。。。。
            foreach (MFactor mf, polIdList) {
                task.req.action = PLCAction::CMD_2062;
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
                task.req.requestSeqNo++;
                emit toPLC(task);
            }

        }
        else if(!isNewDialog && taskSource == TaskDirection::CLOUD_REQUEST){  //处理PLC返回的结果，向云端报送
            //2. 上传监测指标核查数据
            /*格式：
            QN=20160801085857223;ST=21;CN=2062;PW=123456;MN=A110000_0001;Flag=8;CP=&&DataTime=20160801080000;w01001-Check=63.0,w01001-StandardValue=60,
            w01001-Flag=N;w01003-Check=43.0,w01003-StandardValue=40,w01003-Flag=N;w01009-Check=13.0,w01009-StandardValue=10,w01009-Flag=N;…&&
            */


            //3. 返回执行结果，标准格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=A110000_0001;Flag=8;CP=&&ExeRtn=1&&
            QString dpdDataString = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=8;CP=&&ExeRtn=1&&").arg(this->QN1).arg(PW).arg(MN);
            QString msg = pt.CloudCmdBuilder(dpdDataString, &isOK);
            emit toCloud(msg);
            if(task.req.requestSeqNo==this->PNUM)  //请求的次数等于总数，就结束SubWorker2062
                emit isDone(this->QN1);
        }
        else if(isNewDialog && taskSource == TaskDirection::SELF_REPORT){//自报告不需要，因此，不考虑以下代码（可能存在错误，未经仔细斟酌）
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

QString SubWorker2062::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}
