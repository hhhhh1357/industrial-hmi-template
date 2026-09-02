#ifndef BBASE
#define BBASE

#include <QMutexLocker>

#include "Headers/DAL/dal.h"
#include "Headers/Model/MBase.h"

extern Sqlite bSqlite;

class BBase
{
public:
    QString tableName;
    Sqlite * sqlite;

    BBase();
    ~BBase();

    int add(QMap<QString, QString> data);
    int virtual insert(MBase * rec) = 0;
    int del(int id);
    int del(QString code);
    int del(QString field, QString value);
    int virtual update(MBase * rec) = 0;
    bool update(QMap<QString, QString> where, QMap<QString, QString> data);
    int virtual query(QString where, QList<MBase*> * list) = 0;
    int query(QString where, QList<QList<QString>> *row);
    bool isRecordExist(QString code);
    bool isRecordExist(int id);

};

#endif // BBASE

