#ifndef BFACTOR_H
#define BFACTOR_H

#include <QString>
#include <QMutex>
#include <QMutexLocker>

#include "BBase.h"
#include "Headers/Model/MFactor.h"
#include "Headers/BLL/BComArgs.h"

class BFactor:public BBase
{

    int virtual insert(MBase * rec);
    int virtual update(MBase * rec) ;
    int virtual query(QString where, QList<MBase*> * list);
public:
    BFactor();
    ~BFactor();
    bool update(QMap<QString, QString> where, QMap<QString, QString> data);
    int query(QString where, QList<MFactor*> * list);
    QString getNameByCode(QString code);
    QString getDataTypeByCode(QString code);
    bool getFactorInfoByCode(QString code, MFactor & factor);
    QString getArgsByCode(QString code);
    bool saveFactor(MFactor factor);



    static bool findFactor(QString factorCode, MFactor & factor );
};


#endif // BFACTOR_H
