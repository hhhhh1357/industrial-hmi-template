#ifndef POLDATA_H
#define POLDATA_H

#include <QString>
#include "InfoData.h"

#include "SBData.h"

class PolData
{
public:
    PolData();
    PolData(const PolData &obj);
    PolData& operator=(const PolData &obj);
    ~PolData();

    QString toString();
    PolData StrToPolData(QString str);

public:
    QString Rtd; //污染物实时采样数据
    QString Min; //污染物指定时间内最小值
    QString Avg; //污染物指定时间内平均值
    QString Max; //污染物指定时间内最大值
    QString ZsRtd; //污染物实时采样折算数据

    QString ZsMin; //污染物指定时间内最小折算值
    QString ZsAvg; //污染物指定时间内平均折算值
    QString ZsMax; //污染物指定时间内最大折算值
    QString Flag;//监测仪器数据标记
    QString EFlag;//监测仪器扩充数据标记

    QString Cou; //污染物指定时间内累计值
    QString Data;//噪声监测时间段内数据
    QString DayData;//噪声昼间数据
    QString NightData;//噪声夜间数据
    QString SampleTime; //污染物采样时间

    QString DataTime;   //在使用分钟命令2051、小时命令2061、日数据命令2031、2041时，时间标签为测量开始时间；在使用实时数据命令2011、2021等，时间标签为数据采集的时刻；
    QString Check;    //核验值
    QString StandardValue;  //标准值
    QString SN;       //在线监控（监测）仪器仪表编码
    QString PolId;    //污染因子的编码


};




#endif // POLDATA_H
