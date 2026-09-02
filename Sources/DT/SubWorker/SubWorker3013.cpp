//启动清洗/反吹（上位机发起）

#include "SubWorker3013.h"

#include <Headers/BLL/BLocalService.h>

extern BLocalService * localService;

SubWorker3013::SubWorker3013(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent):SubWorkerBase(ip, pw,  mn,sqlTools, parent)
{

}
SubWorker3013::~SubWorker3013()
{
    //qDebug()<<__FILE__ << ", " << __func__<<" was called !"<<endl;
}
void SubWorker3013::doWork(Task task)
{


    bool isValidCmd = true;

    if(isValidCmd){
        TaskDirection taskSource = task.taskSource;  //默认是UI_REQUEST
        DataPacketData dpd = task.dataPacket.data;
        QString QN = dpd.QN;
        QString CN = dpd.CN;
        QString ST = dpd.ST;
        QString polId= dpd.PolId;

        //1. 返回请求应答(标准中的回复命令格式)
        //QN=20160801085857223;ST=91;CN=9011;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&QnRtn=1&&
        QString msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&QnRtn=%7&&").arg(QN).arg("91").arg("9011").arg(PW).arg(MN).arg("4").arg("1");
        emit toCloud(msg);

        //2. 发送消息给DB或UI，启动
        localService->startUploadRealData();

        //3. 返回执行结果
        //QN=20160801085857223;ST=91;CN=9012;PW=123456;MN=010000A8900016F000169DC0;Flag=4;CP=&&ExeRtn=1&&   (标准中的回复命令格式)
        msg = QString("QN=%1;ST=%2;CN=%3;PW=%4;MN=%5;Flag=%6;CP=&&ExeRtn=%7&&").arg(QN).arg("91").arg("9012").arg(PW).arg(MN).arg("4").arg("1");
        emit toCloud(msg);
    }
}

QString SubWorker3013::buildCloudCmd(Task &task, bool &shouldDone)
{
    return "";

}
