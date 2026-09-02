#ifndef REQUEST_H
#define REQUEST_H

#include <QDateTime>
#include <QObject>
#include "uart.h"
#include "DataAlign.h"
#include "PLCAction.h"

typedef enum{
    POSITIVE = 0,      //正逻辑
    NEGATIVE =1      //反逻辑
}RELAYER_LOGIC;

class PLCRequest
{
    //Q_OBJECT
public:
    //explicit Request(QObject *parent = nullptr);
    PLCRequest();

signals:

public:

        URT_PORT port;

        SERIAL_PORT_CONFIG config;
        QString factorCode; //监测因子编号
        unsigned char stationNo;   //站号
        unsigned char cmdCode;    //命令码
        quint16 valueCount;  //取值个数
        quint16 plcAddr;    //PLC地址
        DataAlign dataAlign; //大小端

        //QString
        PLCAction action;   //任务的动作（0：读取数值； 1： 设置参数；2：执行一个动作）；
        //用于反控
        RELAYER_CHANNEL ch1;    //当用于继电器时，告警（拉低）
        RELAYER_CHANNEL ch2;   //当用于继电器时，计数（拉低并开始计时，3秒后再拉高）
        RELAYER_LOGIC logic;    //正逻辑或反逻辑

        QString ip;   //云端IP，以便掌握请求的来源，在每个具体分拆的任务处理函数中需要判断是否是自己处理的

        int repeats=3;    //重发次数

        int requestSeqNo=0;   //对于往返的同一个Task对象，该属性参数记录的是请求的序号数（表示是第几次请求）

        QDateTime taskBegin; //任务开始时间，便于UI端进行统计
};

#endif // REQUEST_H
