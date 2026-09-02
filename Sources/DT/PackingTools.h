#ifndef PACKINGTOOLS_H
#define PACKINGTOOLS_H

#include "CloudDataPacket.h"
#include "DataPacketData.h"

#include <QMap>
#include <QObject>
#include <QDebug>

class PackingTools : public QObject
{
    Q_OBJECT
public:
    explicit PackingTools(QObject *parent = nullptr);

    static QMap<QString, QString> Split(QString msg);

    //参数，接收云端字符串msg（包含包头2字符、数据段长度4字符、包尾巴2字符）
    CloudDataPacket CloudDataPacketBuilder(QString strCloudDataPacket);

    //根据一个DataPacketData构建一个发往云端的命令串
    QString CloudCmdBuilder(DataPacketData data, bool *isOk);

    //根据一个数据段字符串构造一个命令串
    QString CloudCmdBuilder(QString dataPacketDataString, bool *isOk);

    quint16 CRC16(unsigned char * data, int len);    //ModuBus CRC

    unsigned int CRC16_Checkout ( unsigned char *puchMsg, unsigned int usDataLen); //Cloud CRC

    //根据键值对QMap对象构建CmdParas
    CmdParas CmdParasBuilder(QMap<QString, QString> dataMap);
    //根据DataPacketData的内容字符串strDataPacketData构建DataPacketData;
    DataPacketData DataPacketDataBuilder(QString strDataPacketData);

    PolData StrToPolData(QString str);
    SBData  StrToSBData(QString str);
    InfoData StrToInfoData(QString str);
};


#endif // PACKINGTOOLS_H

