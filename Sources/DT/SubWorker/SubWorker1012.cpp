//1012 设置现场机时间（上位机发起）

#include "SubWorker1012.h"

#include <Headers/BLL/BLocalService.h>
extern BLocalService * localService;
SubWorker1012::SubWorker1012(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw,  mn,sqlTools, parent)
{

}

SubWorker1012::~SubWorker1012()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker1012::doWork(Task task)
{
    bool isValidCmd = true;
    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString polId= dpd.PolId;
        QString SystemTime = dpd.CP.SystemTime;

        this->QN1 = QN;
        sqlTools->newDialogRecord(this->QN1, (qlonglong)this, taskSource, this->IP);
        //1. 返回请求应答(标准中的回复命令格式)
        //QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
        QString msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&QnRtn=%7&&").arg(QN).arg("91").arg("9011").arg(PW).arg(MN).arg("4").arg("1");
        bool isOK=false;
        PackingTools pt;
        msg = pt.CloudCmdBuilder(msg, &isOK);

        emit toCloud(msg);
        //2. 执行。调用或发消息（设置数采仪现场机时间）
        if (polId=="")  //设置数采仪的现场时间
        {
            localService->setLocalDatetime(SystemTime);
        }
        else{//否则设置PLC的时间（上位机发送设置PLC时间请求的响应）
            //2.向PLC请求“设置PLC时间”
            MFactor mf;
            bool r = localService->findFactor(polId, mf);//找到检测污染因子所在的端口信息
            if(r){
                task.req.action = PLCAction::SET_PLC_TIME;
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

                //connect(this, SIGNAL(toPLC(Task)), &plc, SLOT(requestHandler(Task)));
                emit toPLC(task);
            }
        }

        //3. 返回执行结果
        //QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&   (标准中的回复命令格式)

        msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&ExeRtn=%7&&").arg(QN).arg("91").arg("9012").arg(PW).arg(MN).arg("4").arg("1");
        emit toCloud(msg);
        emit isDone(this->QN1);
    }
}

QString SubWorker1012::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}
