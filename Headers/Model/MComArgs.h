#ifndef MCOMARGS_H
#define MCOMARGS_H

#include <QString>
#include <QList>
#include <QJsonObject>

#include "MBase.h"
#include "Headers/Model/MFactor.h"

class MComArgs: public MBase
{
public:
    int CId;
    QString PortName;       //端口名称: RS232-1、RS232-2
    QString PortType;       //端口类型: RS232、RS485、A、N、S、R
    QString PortTypeName;   //端口类型名：数字量通道、模拟量通道
    COM_STATUS Status=NORMAL;  //端口状态

    //串口配置参数
    int Speed = 9600;              //端口波特率
    int DataBit = 8;            //数据位
    int StopBit = 1;            //停止位
    int CheckBit = CHECK_EVEN;           //校验位，校验方式

    //模拟量配置参数
    SIGNAL_TYPE SignalType = VOLTAGE; //信号类型默认为电压
    double DataRangeMax;  //量程最大值
    double DataRangeMin;  //量程最小值
    double FactorDataRangeMax;//因子值最大值
    double FactorDataRangeMin;//因子值最小值

    //开关量
    LOGIC_TYPE LogicType = POSITIVE_LOGIC; //开关量信号逻辑类型

    //继电器
    CH_STATUS_TYPE StatusType = ALARM; //通道状态默认为告警

    QString Args;

    QList<MFactor> ComFactorArgsList;

    bool isUpdate;
};


#endif // MCOMARGS_H
