#include "SBData.h"

SBData::SBData(QString rt, QString rs,QString sbId):RT(rt),RS(rs),SBId(sbId)
{

}
SBData::SBData(const SBData &obj)
{
    this->RS = obj.RS;
    this->RT = obj.RT;
    this->SBId = obj.SBId;
}

SBData &SBData::operator=(const SBData &obj)
{
    this->RS = obj.RS;
    this->RT = obj.RT;
    this->SBId = obj.SBId;
}

SBData::~SBData()
{

}
