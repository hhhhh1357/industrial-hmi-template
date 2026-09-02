#ifndef MSTATISTICALDATA_H
#define MSTATISTICALDATA_H

#include <QString>


class MStatisticalData
{
public:
    QString id;
    QString code;
    QString dataType="M";
    QString timestamp="2021-11-01 00:00";
    double valueCou;
    double valueMax;
    double valueMin;
    double valueAvg;
    QString dataFlag="N";
    int flag=0;
};

#endif // MSTATISTICALDATA_H
