#ifndef CLOUDDATAPACKET_H
#define CLOUDDATAPACKET_H

#include <QObject>
#include "DataPacketData.h"
//通信包（与云端服务器交互的数据包对象）

class CloudDataPacket : public QObject
{
    Q_OBJECT
public:
    explicit CloudDataPacket(QObject *parent = nullptr);

    CloudDataPacket(const CloudDataPacket &obj,QObject *parent = nullptr);
    CloudDataPacket& operator=(const CloudDataPacket &obj);

    QString header="##";
    QString len;
    DataPacketData data;
    QString crc;
    QString tail="\r\n";

signals:

};



#endif // CLOUDDATAPACKET_H
