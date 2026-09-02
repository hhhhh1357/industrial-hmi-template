#include "DataPacketData.h"
//数据包的数据

DataPacketData::DataPacketData(QObject *parent) : QObject(parent)
{

}

DataPacketData::~DataPacketData(){

}

DataPacketData::DataPacketData(const DataPacketData &x)
{
    QN= x.QN;
    ST = x.ST;
    CN = x.CN;
    PW = x.PW;
    MN = x.MN;
    Flag = x.Flag;
    PNUM = x.PNUM;
    PNO= x.PNO;
    CP = x.CP;
    PolId = x.PolId;
    InfoId = x.InfoId;
    SelfReportCmd = x.SelfReportCmd;

}

DataPacketData &DataPacketData::operator=(const DataPacketData &temp)
{
    QN = temp.QN;
    ST = temp.ST;
    CN = temp.CN;
    PW = temp.PW;
    MN = temp.MN;
    Flag = temp.Flag;
    PNUM = temp.PNUM;
    PNO = temp.PNO;
    CP = temp.CP;
    PolId = temp.PolId;
    InfoId = temp.InfoId;
    SelfReportCmd = temp.SelfReportCmd;
    return *this;
}

