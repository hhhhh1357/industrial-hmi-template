#include "InfoData.h"

InfoData::InfoData(QString id, QString info):InfoId(id),Info(info)
{

}
InfoData::InfoData(const InfoData &obj)
{
    this->Info = obj.Info;
    this->InfoId = obj.InfoId;
}

InfoData &InfoData::operator=(const InfoData &obj)
{
    this->Info = obj.Info;
    this->InfoId = obj.InfoId;
}

InfoData::~InfoData()
{

}
