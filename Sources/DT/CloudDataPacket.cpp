#include "CloudDataPacket.h"

//通信包（与云端服务器交互的数据包对象）

CloudDataPacket::CloudDataPacket(QObject *parent) : QObject(parent)
{

}
CloudDataPacket::CloudDataPacket(const CloudDataPacket &obj,QObject *parent):QObject(parent)
{
    this->header = obj.header;

    this->len = obj.len;
    this->data = obj.data;
    this->crc = obj.crc;
    this->tail = obj.tail;

}

CloudDataPacket &CloudDataPacket::operator=(const CloudDataPacket &obj)
{
    this->header = obj.header;

    this->len = obj.len;
    this->data = obj.data;
    this->crc = obj.crc;
    this->tail = obj.tail;

    return *this;
}
