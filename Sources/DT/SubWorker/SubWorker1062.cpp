//1062 设置实时数据间隔（上位机发起）

#include "SubWorker1062.h"

#include <Headers/BLL/BLocalService.h>

extern BLocalService * localService;

SubWorker1062::SubWorker1062(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw,  mn,sqlTools, parent)
{

}

SubWorker1062::~SubWorker1062()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}

void SubWorker1062::doWork(Task task)
{

    //先要验证上位机请求命令是否有效
    bool isValidCmd = true;

    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString RtdInterval = dpd.CP.RtdInterval;

        this->QN1 = QN;
        bool isNewDialog = sqlTools->isNewDialog(dpd.QN, this->IP);
        if(isNewDialog && taskSource==TaskDirection::CLOUD_REQUEST){
            sqlTools->newDialogRecord(QN, (qlonglong)this, taskSource, this->IP);
            //1. 返回请求应答命令(标准中的命令格式：QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&）
            QString msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&QnRtn=%7&&").arg(QN).arg("91").arg("9011").arg(PW).arg(MN).arg("4").arg("1");
            bool isOK=false;
            PackingTools pt;
            msg = pt.CloudCmdBuilder(msg, &isOK);
            emit toCloud(msg);

            //2. 设置实时时钟间隔
            //。。。。调用 API
            localService->setLocalRealDataInterval(RtdInterval.toInt());

            //3. 返回执行结果命令（标准中的命令格式：QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&）
            msg = QString("QN=%1;ST=91;CN=9012;PW=%2;MN=%3;Flag=4;CP=&&ExeRtn=1&&").arg(QN).arg(PW).arg(MN);
            msg = pt.CloudCmdBuilder(msg, &isOK);
            emit toCloud(msg);
            emit isDone(this->QN1);
        }

    }
}

QString SubWorker1062::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";
}
