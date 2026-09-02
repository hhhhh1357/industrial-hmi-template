#ifndef SBDATA_H
#define SBDATA_H

#include <QObject>

class SBData
{

public:
    SBData(QString rt, QString rs,QString sbId);
    SBData(const SBData& obj);
    SBData & operator=(const SBData& obj);
    ~SBData();
    QString RT;
    QString RS;
    QString SBId;  //设备号


};



#endif // SBDATA_H
