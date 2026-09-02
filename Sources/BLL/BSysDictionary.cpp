#include "Headers/BLL/BSysDictionary.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


SysDictionary::SysDictionary()
{
    this->sqlite = &bSqlite;
    this->tableName="TSysDictionary";
    getSysDictionary();
}

void SysDictionary::getSysDictionary()
{
   QList< QList<QString> > * rows = new QList< QList<QString> >();
   QString where="1>0";

   int r;

   {     
      r = this->sqlite->find(SysDictionary::tableName,where,rows);
   }

   for (int i=0;i<rows->size();i++)
   {
       QList<QString> rec = rows->at(i);
       if (rec.at(1)=="MN")
       {
           this->MN.key = rec.at(1);
           this->MN.value = rec.at(2);
           this->MN.dataType = rec.at(3);
           this->MN.memo = rec.at(4);
       }
       else if (rec.at(1)=="ST")
       {
           this->ST.key = rec.at(1);
           this->ST.value = rec.at(2);
           this->ST.dataType = rec.at(3);
           this->ST.memo = rec.at(4);
       }
       else if (rec.at(1)=="TIMEOUT")
       {
           this->TIMEOUT.key = rec.at(1);
           this->TIMEOUT.value = rec.at(2);
           this->TIMEOUT.dataType = rec.at(3);
           this->TIMEOUT.memo = rec.at(4);
       }
       else if (rec.at(1)=="REPEAT")
       {
           this->REPEAT.key = rec.at(1);
           this->REPEAT.value = rec.at(2);
           this->REPEAT.dataType = rec.at(3);
           this->REPEAT.memo = rec.at(4);
       }
       else if (rec.at(1)=="STORAGE_TIME")
       {
           this->STORAGE_TIME.key = rec.at(1);
           this->STORAGE_TIME.value = rec.at(2);
           this->STORAGE_TIME.dataType = rec.at(3);
           this->STORAGE_TIME.memo = rec.at(4);
       }
       else if (rec.at(1)=="INTERVAL")
       {
           this->INTERVAL.key = rec.at(1);
           this->INTERVAL.value = rec.at(2);
           this->INTERVAL.dataType = rec.at(3);
           this->INTERVAL.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_REAL")
       {
           this->DATA_UPLOAD_REAL.key = rec.at(1);
           this->DATA_UPLOAD_REAL.value = rec.at(2);
           this->DATA_UPLOAD_REAL.dataType = rec.at(3);
           this->DATA_UPLOAD_REAL.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_MINUTE")
       {
           this->DATA_UPLOAD_MINUTE.key = rec.at(1);
           this->DATA_UPLOAD_MINUTE.value = rec.at(2);
           this->DATA_UPLOAD_MINUTE.dataType = rec.at(3);
           this->DATA_UPLOAD_MINUTE.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_HOUR")
       {
           this->DATA_UPLOAD_HOUR.key = rec.at(1);
           this->DATA_UPLOAD_HOUR.value = rec.at(2);
           this->DATA_UPLOAD_HOUR.dataType = rec.at(3);
           this->DATA_UPLOAD_HOUR.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_DAY")
       {
           this->DATA_UPLOAD_DAY.key = rec.at(1);
           this->DATA_UPLOAD_DAY.value = rec.at(2);
           this->DATA_UPLOAD_DAY.dataType = rec.at(3);
           this->DATA_UPLOAD_DAY.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_REAL_ENABLED")
       {
           this->DATA_UPLOAD_REAL_ENABLED.key = rec.at(1);
           this->DATA_UPLOAD_REAL_ENABLED.value = rec.at(2);
           this->DATA_UPLOAD_REAL_ENABLED.dataType = rec.at(3);
           this->DATA_UPLOAD_REAL_ENABLED.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_MINUTE_ENABLED")
       {
           this->DATA_UPLOAD_MINUTE_ENABLED.key = rec.at(1);
           this->DATA_UPLOAD_MINUTE_ENABLED.value = rec.at(2);
           this->DATA_UPLOAD_MINUTE_ENABLED.dataType = rec.at(3);
           this->DATA_UPLOAD_MINUTE_ENABLED.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_HOUR_ENABLED")
       {
           this->DATA_UPLOAD_HOUR_ENABLED.key = rec.at(1);
           this->DATA_UPLOAD_HOUR_ENABLED.value = rec.at(2);
           this->DATA_UPLOAD_HOUR_ENABLED.dataType = rec.at(3);
           this->DATA_UPLOAD_HOUR_ENABLED.memo = rec.at(4);
       }
       else if (rec.at(1)=="DATA_UPLOAD_DAY_ENABLED")
       {
           this->DATA_UPLOAD_DAY_ENABLED.key = rec.at(1);
           this->DATA_UPLOAD_DAY_ENABLED.value = rec.at(2);
           this->DATA_UPLOAD_DAY_ENABLED.dataType = rec.at(3);
           this->DATA_UPLOAD_DAY_ENABLED.memo = rec.at(4);
       }
       else if (rec.at(1)=="TABLE_LENGTH")
       {
           this->TABLE_LENGTH.key = rec.at(1);
           this->TABLE_LENGTH.value = rec.at(2);
           this->TABLE_LENGTH.dataType = rec.at(3);
           this->TABLE_LENGTH.memo = rec.at(4);
       }
       else if (rec.at(1)=="DOG_TIMER")
       {
           this->DOG_TIMER.key = rec.at(1);
           this->DOG_TIMER.value = rec.at(2);
           this->DOG_TIMER.dataType = rec.at(3);
           this->DOG_TIMER.memo = rec.at(4);
       }
       else if (rec.at(1)=="SCREEN_SAVE_TIME")
       {
           this->SCREEN_SAVE_TIME.key = rec.at(1);
           this->SCREEN_SAVE_TIME.value = rec.at(2);
           this->SCREEN_SAVE_TIME.dataType = rec.at(3);
           this->SCREEN_SAVE_TIME.memo = rec.at(4);
       }
       else if (rec.at(1)=="PING_SERVERS")
       {
           this->PING_SERVERS.key = rec.at(1);
           this->PING_SERVERS.value = rec.at(2);
           this->PING_SERVERS.dataType = rec.at(3);
           this->PING_SERVERS.memo = rec.at(4);
       }
   }
   delete rows;
}

/*
bool SysDictionary::putSysDictionary(QString MN, QString ST, int TIMEOUT, int REPEAT,
                                     int STORAGE_TIME, int INTERVAL, int DATA_UPLOAD_REAL,
                                     int DATA_UPLOAD_MINUTE, int DATA_UPLOAD_HOUR,
                                     bool DATA_UPLOAD_REAL_ENABLED)
{
    bool r1 = putMN(MN);
    bool r2 = putTIMEOUT(TIMEOUT);
    bool r3 = putREPEAT(REPEAT);
    bool r4 = putSTORAGE_TIME(STORAGE_TIME);
    bool r5 = putINTERVAL(INTERVAL);
    bool r6 = putDATA_UPLOAD_REAL(DATA_UPLOAD_REAL);
    bool r7 = putDATA_UPLOAD_MINUTE(DATA_UPLOAD_MINUTE);
    bool r8 = putDATA_UPLOAD_HOUR(DATA_UPLOAD_HOUR);
    bool r9 = putST(ST);
    bool r10 = putDATA_UPLOAD_REAL_ENABLED(DATA_UPLOAD_REAL_ENABLED);
    return (r1&&r2&&r3&&r4&&r5&&r6&&r7&&r8&&r9&&r10);
} */

QString SysDictionary::getMN()
{
    return this->MN.value;
}

bool SysDictionary::putMN(QString MN)
{
    this->MN.value = MN;
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'MN'");
    data.insert("value","'"+this->MN.value+"'");

    return this->sqlite->update(this->tableName, where, data);

}

QString SysDictionary::getST()
{
    return  this->ST.value;
}

bool SysDictionary::putST(QString ST)
{
    this->ST.value = ST;
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'ST'");
    data.insert("value","'"+this->ST.value+"'");

    return this->sqlite->update(this->tableName, where, data);

}

int SysDictionary::getTIMEOUT()
{
    return this->TIMEOUT.value.toInt();
}

bool SysDictionary::putTIMEOUT(int TIMEOUT)
{
    this->TIMEOUT.value = QString::number(TIMEOUT);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'TIMEOUT'");
    data.insert("value","'"+this->TIMEOUT.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getREPEAT()
{
    return this->REPEAT.value.toInt();
}

bool SysDictionary::putREPEAT(int REPEAT)
{
    this->REPEAT.value = QString::number(REPEAT);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'REPEAT'");
    data.insert("value","'"+this->REPEAT.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getSTORAGE_TIME()
{
    return this->STORAGE_TIME.value.toInt();
}

bool SysDictionary::putSTORAGE_TIME(int STORAGE_TIME)
{
    this->STORAGE_TIME.value = QString::number(STORAGE_TIME);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'STORAGE_TIME'");
    data.insert("value","'"+this->STORAGE_TIME.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getINTERVAL()
{
    return this->INTERVAL.value.toInt();
}

bool SysDictionary::putINTERVAL(int INTERVAL)
{
    this->INTERVAL.value = QString::number(INTERVAL);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'INTERVAL'");
    data.insert("value","'"+this->INTERVAL.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getDATA_UPLOAD_REAL()
{
    return this->DATA_UPLOAD_REAL.value.toInt();
}

bool SysDictionary::putDATA_UPLOAD_REAL(int DATA_UPLOAD_REAL)
{
    this->DATA_UPLOAD_REAL.value = QString::number(DATA_UPLOAD_REAL);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_REAL'");
    data.insert("value","'"+this->DATA_UPLOAD_REAL.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getDATA_UPLOAD_MINUTE()
{
    return this->DATA_UPLOAD_MINUTE.value.toInt();
}

bool SysDictionary::putDATA_UPLOAD_MINUTE(int DATA_UPLOAD_MINUTE)
{
    this->DATA_UPLOAD_MINUTE.value = QString::number(DATA_UPLOAD_MINUTE);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_MINUTE'");
    data.insert("value","'"+this->DATA_UPLOAD_MINUTE.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getDATA_UPLOAD_HOUR()
{
    return this->DATA_UPLOAD_HOUR.value.toInt();
}

bool SysDictionary::putDATA_UPLOAD_HOUR(int DATA_UPLOAD_HOUR)
{
    this->DATA_UPLOAD_HOUR.value = QString::number(DATA_UPLOAD_HOUR);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_HOUR'");
    data.insert("value","'"+this->DATA_UPLOAD_HOUR.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getDATA_UPLOAD_DAY()
{
    return this->DATA_UPLOAD_DAY.value.toInt();
}

bool SysDictionary::putDATA_UPLOAD_DAY(int DATA_UPLOAD_DAY)
{
    this->DATA_UPLOAD_DAY.value = QString::number(DATA_UPLOAD_DAY);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_DAY'");
    data.insert("value","'"+this->DATA_UPLOAD_DAY.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

bool SysDictionary::getDATA_UPLOAD_REAL_ENABLED()
{
    if (this->DATA_UPLOAD_REAL_ENABLED.value=="1")
        return true;
    else
        return false;
}

bool SysDictionary::putDATA_UPLOAD_REAL_ENABLED(bool enabled)
{
    if (enabled)
        this->DATA_UPLOAD_REAL_ENABLED.value="1";
    else
        this->DATA_UPLOAD_REAL_ENABLED.value="0";

    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_REAL_ENABLED'");
    data.insert("value","'"+this->DATA_UPLOAD_REAL_ENABLED.value+"'");

    return this->sqlite->update(this->tableName, where, data);

}

bool SysDictionary::getDATA_UPLOAD_MINUTE_ENABLED()
{
    if (this->DATA_UPLOAD_MINUTE_ENABLED.value=="1")
        return true;
    else
        return false;
}

bool SysDictionary::putDATA_UPLOAD_MINUTE_ENABLED(bool enabled)
{
    if (enabled)
        this->DATA_UPLOAD_MINUTE_ENABLED.value="1";
    else
        this->DATA_UPLOAD_MINUTE_ENABLED.value="0";

    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_MINUTE_ENABLED'");
    data.insert("value","'"+this->DATA_UPLOAD_MINUTE_ENABLED.value+"'");

    return this->sqlite->update(this->tableName, where, data);

}

bool SysDictionary::getDATA_UPLOAD_HOUR_ENABLED()
{
    if (this->DATA_UPLOAD_HOUR_ENABLED.value=="1")
        return true;
    else
        return false;
}

bool SysDictionary::putDATA_UPLOAD_HOUR_ENABLED(bool enabled)
{
    if (enabled)
        this->DATA_UPLOAD_HOUR_ENABLED.value="1";
    else
        this->DATA_UPLOAD_HOUR_ENABLED.value="0";

    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_HOUR_ENABLED'");
    data.insert("value","'"+this->DATA_UPLOAD_HOUR_ENABLED.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

bool SysDictionary::getDATA_UPLOAD_DAY_ENABLED()
{
    if (this->DATA_UPLOAD_DAY_ENABLED.value=="1")
        return true;
    else
        return false;
}

bool SysDictionary::putDATA_UPLOAD_DAY_ENABLED(bool enabled)
{
    if (enabled)
        this->DATA_UPLOAD_DAY_ENABLED.value="1";
    else
        this->DATA_UPLOAD_DAY_ENABLED.value="0";

    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DATA_UPLOAD_DAY_ENABLED'");
    data.insert("value","'"+this->DATA_UPLOAD_DAY_ENABLED.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getTABLE_LENGTH()
{
    return this->TABLE_LENGTH.value.toInt();
}

bool SysDictionary::putTABLE_LENGTH(int TableLength)
{
    this->TABLE_LENGTH.value = QString::number(TableLength);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'TABLE_LENGTH'");
    data.insert("value","'"+this->TABLE_LENGTH.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getDOG_TIMER()
{
    return this->DOG_TIMER.value.toInt();
}

bool SysDictionary::putDOG_TIMER(int DOG_TIMER)
{
    this->DOG_TIMER.value = QString::number(DOG_TIMER);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'DOG_TIMER'");
    data.insert("value","'"+this->DOG_TIMER.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getSCREEN_SAVE_TIME()
{
    return this->SCREEN_SAVE_TIME.value.toInt();

}

bool SysDictionary::putSCREEN_SAVE_TIME(int SCREEN_SAVE_TIME)
{
    this->SCREEN_SAVE_TIME.value = QString::number(SCREEN_SAVE_TIME);
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'SCREEN_SAVE_TIME'");
    data.insert("value","'"+this->SCREEN_SAVE_TIME.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

QString SysDictionary::getPING_SERVERS()
{
    return this->PING_SERVERS.value;
}

bool SysDictionary::putPING_SERVERS(QString jsonStr)
{
    this->PING_SERVERS.value = jsonStr;
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("key","'PING_SERVERS'");
    data.insert("value","'"+this->PING_SERVERS.value+"'");

    return this->sqlite->update(this->tableName, where, data);
}

int SysDictionary::getPING_SERVERS_Interval()
{
    QString jsonStr = getPING_SERVERS();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonStr.toLocal8Bit().data());
    if (!(jsonDocument.isNull()))
    {
        QJsonObject objJson = jsonDocument.object();
        QString iStr = objJson.value("Interval").toString();
        return iStr.toInt();
    }
}

QList<QString> SysDictionary::getPING_SERVERS_List()
{
    QList<QString> serverList;

    QString jsonStr = getPING_SERVERS();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonStr.toLocal8Bit().data());
    if (!(jsonDocument.isNull()))
    {
        QJsonObject objJson = jsonDocument.object();
        QString serverListJsonStr = objJson.value("ServerList").toString();
        jsonDocument = QJsonDocument::fromJson(serverListJsonStr.toLocal8Bit().data());

        if (!(jsonDocument.isNull()))
        {
            QJsonArray jsonArray = jsonDocument.array();
            if ( !(jsonArray.isEmpty()) && jsonArray.count()>0)
            {
                for (int j=0;j<jsonArray.size();j++)
                {
                    serverList.append( jsonArray.at(j).toString() );
                }
            }
        }
    }
    return serverList;
}
