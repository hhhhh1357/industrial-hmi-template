#ifndef DATAPACKETDATA_H
#define DATAPACKETDATA_H

//与云端交互的数据包中的数据对象（包含请求码QN、系统编码ST、命令码CN、访问密码PW、设备唯一标识MN、拆分包及应答标志Flag、总包数PNUM、包号PNo、指令参数CP.....)


#include <QObject>
#include "CmdParas.h"

class DataPacketData : public QObject
{
    Q_OBJECT
public:
    explicit DataPacketData(QObject *parent = nullptr);
    DataPacketData(const DataPacketData &x);

    DataPacketData& operator=(const DataPacketData& temp);
    ~DataPacketData();

public:
    QString QN;
    QString ST;  //系统编码
    QString CN;  //命令编码
    QString PW;  //命令编码
    QString MN;  //设备编码

    QString Flag;   //拆分包及应答标志
    QString PNUM;//总包数
    QString PNO; //包号

    QString PolId;  //在线监控（监测）仪器仪表对应污染物编码
    QString InfoId; //
    CmdParas CP;  //指令参数

    QString SelfReportCmd = "";    //主动上报的字符串
};

#endif // DATAPACKETDATA_H
