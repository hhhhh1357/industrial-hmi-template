#ifndef BHJ_T_212_2005_H
#define BHJ_T_212_2005_H

#include <QString>

#include "BBase.h"
#include "BDataItem.h"
#include "Headers/Model/MHJ_T_212_2005.h"

class BHJ_T_212_2005: public BBase
{
public:
    BHJ_T_212_2005();

    int virtual insert(MBase * rec);
    int virtual update(MBase * rec) ;
    int virtual query(QString where, QList<MBase*> * list);
    int query(QString where, QList<MHJ_T_212_2005*> * list);
    bool getInfoByCode(MHJ_T_212_2005* rec, QString code);
    QString getArgsByCode(QString code);
    bool updateArgsByCode(QString args, QString code);
    DataItem getDataItemByCode(QString code); //通过code获得数据项对象
};

#endif // BHJ_T_212_2005_H
