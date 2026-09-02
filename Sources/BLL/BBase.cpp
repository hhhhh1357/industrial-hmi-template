#include "Headers/BLL/BBase.h"

BBase::BBase()
{
    sqlite = &bSqlite;
}

BBase::~BBase()
{

}

int BBase::add(QMap<QString, QString> data)
{
    sqlite->add(tableName, data);
}

bool BBase::isRecordExist(QString code)
{
    QString where="code='"+code+"'";
    if (sqlite->recordCount(tableName,where)>0)
        return true;
    else
        return false;
}

bool BBase::isRecordExist(int id)
{
    QString where="id="+id;
    if (sqlite->recordCount(tableName,where)>0)
        return true;
    else
        return false;
}

int BBase::del(int id)
{
    QString where="id="+id;
    return sqlite->del(tableName,where);
}

int BBase::del(QString code)
{
    QString where="code='"+code+"'";
    return sqlite->del(tableName,where);
}

int BBase::del(QString field, QString value)
{
    QString where=field +"='"+value+"'";
    return sqlite->del(tableName,where);
}

int BBase::query(QString where, QList< QList<QString> > *row)
{
    return sqlite->find(tableName,where,row);
}

bool BBase::update(QMap<QString, QString> where, QMap<QString, QString> data)
{
    return sqlite->update(this->tableName, where, data);
}



