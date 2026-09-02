#ifndef RESPONSE_H
#define RESPONSE_H

#include <QMap>
#include <QObject>
#include "PLCRequest.h"


class PLCResponse //: public QObject
{
    //Q_OBJECT
public:
    //explicit Response(QObject *parent = nullptr);
    PLCResponse();
signals:

public:
    QString value;   //返回汉字
    bool result;

    unsigned char answer[512];//记录的是从PLC返回来的所有数据，包括从站号....检验码
    int answerLen;   //从PLC中返回的数据串长度
    int dataLen;     //实际数据长度

    unsigned char data[512];  //实际数据
    unsigned char crcData[2];

    union dd
    {
        float v;
        unsigned char c[4];
    } singleData;   //返回单个值的时候

    QMap<QString,QString> resultMap;   //返回多个值的时候

    QString ip;   //云端IP，以便响应的去向，在每个具体分拆的任务处理函数中需要判断是否是自己处理并向云端发送的数据




};



#endif // RESPONSE_H
