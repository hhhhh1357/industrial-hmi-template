#ifndef CP_H
#define CP_H

//通信包中指令参数
#include "PolData.h"

#include <QObject>

class CmdParas : public QObject
{
   // Q_OBJECT
public:
    explicit CmdParas(QObject *parent = nullptr);
    CmdParas(const CmdParas &obj,QObject *parent = nullptr);
    CmdParas& operator=(const CmdParas& temp);
public:
    QString SystemTime;  //系统时间
    QString QnRtn;    //请求回应
    QString ExeRtn;   //执行结果回应
    QString RtdInterval;   //实时采样数据上报
    QString MinInterval;   //分钟数据上报间隔
    QString RestartTime;   //数采仪开机时间


    QList<PolData> polDataList;
    QList<SBData> sbDataList;  //污染治理设施运行状态取值；//污染治理设施一日内运行时间
    QList<InfoData> infoDataList;//现场端编码信息

    QString BeginTime;//开始时间
    QString EndTime;  //截止时间
    QString DataTime; //数据时间信息
    QString NewPW;    //新密码
    QString OverTime; //超时时间
    QString ReCount;  //重发次数
    QString VaseNo;   //采样瓶编号
    QString CstartTime;//设备采样起始时间
    QString Ctime;    //采样周期
    QString Stime;    //出样时间

    QString SN; //在线监控（监测）仪器仪表编码


signals:

};





#endif // CP_H
