#include "PolData.h"
#include "PackingTools.h"
PolData::PolData()
{

}
PolData::PolData(const PolData &obj)
{
    this->Avg = obj.Avg;
    this->Max = obj.Max;
    this->Min = obj.Min;
    this->Rtd = obj.Rtd;
    this->Data = obj.Data;
    this->Flag = obj.Flag;
    this->EFlag = obj.EFlag;
    this->PolId = obj.PolId;
    this->Cou = obj.Cou;
    this->ZsAvg = obj.ZsAvg;

    this->ZsMax = obj.ZsMax;
    this->ZsMin = obj.ZsMin;
    this->ZsRtd = obj.ZsRtd;
    this->DayData = obj.DayData;
    this->NightData = obj.NightData;
    this->SampleTime = obj.SampleTime;
    this->DataTime = obj.DataTime;
    this->Check = obj.Check;
    this->StandardValue = obj.StandardValue;
    this->SN = obj.SN;
}

PolData &PolData::operator=(const PolData &obj)
{
    this->Avg = obj.Avg;
    this->Max = obj.Max;
    this->Min = obj.Min;
    this->Rtd = obj.Rtd;
    this->Data = obj.Data;
    this->Flag = obj.Flag;
    this->EFlag = obj.EFlag;
    this->PolId = obj.PolId;
    this->Cou = obj.Cou;
    this->ZsAvg = obj.ZsAvg;

    this->ZsMax = obj.ZsMax;
    this->ZsMin = obj.ZsMin;
    this->ZsRtd = obj.ZsRtd;
    this->DayData = obj.DayData;
    this->NightData = obj.NightData;
    this->SampleTime = obj.SampleTime;
    this->DataTime = obj.DataTime;
    this->Check = obj.Check;
    this->StandardValue = obj.StandardValue;
    this->SN = obj.SN;
    return *this;
}

PolData::~PolData()
{

}
QString PolData::toString()
{
    QString rs = QString("%1-Rtd=%2;%3-Min=%4;%5-Avg=%6;%7-Max=%8;%9-ZsRtd=%10;%11-ZsMin=%12;%13-ZsAvg=%14;%15-ZsMax=%16;%17-Flag=%18;%19-EFlag=%20;%21-Cou=%22;%23-Data=%24;%25-DayData=%26;%27-NightData=%28;%29-SampleTime=%30;%31-DataTime=%32;%33-Check=%34;%35-StandardValue=%36;%37-SN=%38;PolId=%39;")
                .arg(PolId).arg(Rtd).arg(PolId).arg(Min).arg(PolId).arg(Avg).arg(PolId).arg(Max).arg(PolId).arg(ZsRtd)
                .arg(PolId).arg(ZsMin).arg(PolId).arg(ZsAvg).arg(PolId).arg(ZsMax).arg(PolId).arg(Flag).arg(PolId).arg(EFlag)
                .arg(PolId).arg(Cou).arg(PolId).arg(Data).arg(PolId).arg(DayData).arg(PolId).arg(NightData).arg(PolId).arg(SampleTime)
                .arg(PolId).arg(DataTime).arg(PolId).arg(Check).arg(PolId).arg(StandardValue).arg(PolId).arg(SN).arg(PolId);
    //qDebug()<<__FILE__<<"  "<<__func__<<"   rs="<<rs<<endl;
    return rs;
}

PolData PolData::StrToPolData(QString str)
{
    PackingTools pt;

    QMap<QString, QString> dataMap = pt.Split(str);
    PolData polData;
    polData.PolId = dataMap.value("PolId","");
    polData.SN = dataMap.value(polData.PolId+"-SN", "");
    polData.Avg = dataMap.value(polData.PolId+"-Avg", "");
    polData.Cou = dataMap.value(polData.PolId+"-Cou", "");
    polData.Max = dataMap.value(polData.PolId+"-Max", "");

    polData.Min = dataMap.value(polData.PolId+"-Min", "");
    polData.Rtd = dataMap.value(polData.PolId+"-Rtd", "");
    polData.Data = dataMap.value(polData.PolId+"-Data", "");
    polData.Flag = dataMap.value(polData.PolId+"-Flag", "");
    polData.Check = dataMap.value(polData.PolId+"-Check", "");

    polData.EFlag = dataMap.value(polData.PolId+"-EFlag", "");
    polData.ZsAvg = dataMap.value(polData.PolId+"-ZsAvg", "");
    polData.ZsMax = dataMap.value(polData.PolId+"-ZsMax", "");
    polData.ZsMin = dataMap.value(polData.PolId+"-ZsMin", "");
    polData.ZsRtd = dataMap.value(polData.PolId+"-ZsRtd", "");

    polData.DayData = dataMap.value(polData.PolId+"-DayData", "");
    polData.DataTime = dataMap.value(polData.PolId+"-DataTime", "");
    polData.NightData = dataMap.value(polData.PolId+"-NightData", "");
    polData.SampleTime = dataMap.value(polData.PolId+"-SampleTime", "");
    polData.StandardValue = dataMap.value(polData.PolId+"-StandardValue", "");


//    DataPacketData dpd = pt.DataPacketDataBuilder(str);

//    PolData polData = dpd.CP.polDataList[0];
    return polData;
}
