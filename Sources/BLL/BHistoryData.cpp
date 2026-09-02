#include "Headers/BLL/BHistoryData.h"

#include <Headers/BLL/BComArgs.h>
#include <Headers/BLL/BFactor.h>
#include <Headers/BLL/BSysConfig.h>
#include <Headers/BLL/BSysDictionary.h>
#include <QDebug>
#include <Sources/Tools/datatype.h>

extern BComArgs bca;

bool BHistoryData::getHistoryData(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd,
                                  int limit, int offset, QVector<double> &valuesMax, QVector<double> &valuesMin,
                                  QVector<double> &valuesAvg, QVector<double> &timestamps)
{
    QList< QList<QString> > rows;

    QString dtFormat,dType;

    if (dataType=='H')
    {
        dtFormat = "yyyy-MM-dd hh";
        dType ="H";
    }
    else if (dataType=='D')
    {
        dtFormat = "yyyy-MM-dd";
        dType ="D";
    }
    else  //默认取分钟数据
    {
        dtFormat = "yyyy-MM-dd hh:mm";
        dType ="M";
    }

    QString sql = "SELECT timestamp, valueMax, valueMin, valueAvg FROM TStatisticalData";

    sql += " WHERE timestamp<='"+timeEnd.toString(dtFormat)+"' and timestamp>='"+timeStart.toString(dtFormat)+"'";
    sql += " AND code='"+factorCode+"' AND dataType='"+dType+"'";
    sql += " ORDER BY timestamp LIMIT "+ QString::number(limit)+" OFFSET "+QString::number(offset);

    bool r = bSqlite.find(sql, &rows);
//qDebug()<<"SQL:"<<sql<<endl;
    if (r==false || rows.count()==0)
        return false;

    QDateTime dt;
    for (int i=0;i<rows.count();i++)
    {
        dt = QDateTime::fromString(rows.at(i).at(0),dtFormat);
        timestamps.append(dt.toMSecsSinceEpoch());
        valuesMax.append(rows.at(i).at(1).toDouble());
        valuesMin.append(rows.at(i).at(2).toDouble());
        valuesAvg.append(rows.at(i).at(3).toDouble());
    }
    return true;
}

bool BHistoryData::getHistoryData(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd, QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg, QVector<double> &timestamps)
{
    QList< QList<QString> > rows;

    QString dtFormat,dType;

    if (dataType=='H')
    {
        dtFormat = "yyyy-MM-dd hh";
        dType ="H";
    }
    else if (dataType=='D')
    {
        dtFormat = "yyyy-MM-dd";
        dType ="D";
    }
    else  //默认取分钟数据
    {
        dtFormat = "yyyy-MM-dd hh:mm";
        dType ="M";
    }

    QString sql = "SELECT timestamp, valueMax, valueMin, valueAvg FROM TStatisticalData";

    sql += " WHERE timestamp<='"+timeEnd.toString(dtFormat)+"' and timestamp>='"+timeStart.toString(dtFormat)+"'";
    sql += " AND code='"+factorCode+"' AND dataType='"+dType+"'";
    sql += " ORDER BY timestamp";

    bool r = bSqlite.find(sql, &rows);

    if (r==false || rows.count()==0)
        return false;

    QDateTime dt;
    for (int i=0;i<rows.count();i++)
    {
        dt = QDateTime::fromString(rows.at(i).at(0),dtFormat);
        timestamps.append(dt.toMSecsSinceEpoch());
        valuesMax.append(rows.at(i).at(1).toDouble());
        valuesMin.append(rows.at(i).at(2).toDouble());
        valuesAvg.append(rows.at(i).at(3).toDouble());
    }
    return true;
}

bool BHistoryData::getHistoryData(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data)
{
    QList< QList<QString> > rows;

    QString dtFormat,dType;

    if (dataType=="H")
    {
        dtFormat = "yyyy-MM-dd hh";
        dType ="H";
    }
    else if (dataType=="D")
    {
        dtFormat = "yyyy-MM-dd";
        dType ="D";
    }
    else  //默认取分钟数据
    {
        dtFormat = "yyyy-MM-dd hh:mm";
        dType ="M";
    }

    QString sql = "SELECT * FROM TStatisticalData";

    sql += " WHERE timestamp<='"+timeEnd.toString(dtFormat)+"' and timestamp>='"+timeStart.toString(dtFormat)+"'";
    sql += " AND code='"+factorCode+"' AND dataType='"+dType+"'";
    sql += " ORDER BY timestamp";

    bool r = bSqlite.find(sql, &rows);
//qDebug()<<"SQL："<<sql<<endl;
    if (r==false || rows.count()==0)
        return false;

    QDateTime dt;
    for (int i=0;i<rows.count();i++)
    {
        dt = QDateTime::fromString(rows.at(i).at(0),dtFormat);
//qDebug()<<"timestamp:"<<rows.at(i).at(0)<<endl;
        MStatisticalData msd;
        msd.id = rows.at(i).at(0);
        msd.code = factorCode;
        msd.dataType = dataType;
        msd.timestamp = rows.at(i).at(3);
        msd.valueCou = rows.at(i).at(4).toDouble();
        msd.valueMax = rows.at(i).at(5).toDouble();
        msd.valueMin = rows.at(i).at(6).toDouble();
        msd.valueAvg = rows.at(i).at(7).toDouble();
        msd.dataFlag = rows.at(i).at(8);
        msd.flag = rows.at(i).at(9).toInt();
        data.append(msd);
    }
    return true;
}

int BHistoryData::getHistoryDataLength(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd)
{
    QString dtFormat,dType;

    if (dataType=='H')
    {
        dtFormat = "yyyy-MM-dd hh";
        dType ="H";
    }
    else if (dataType=='D')
    {
        dtFormat = "yyyy-MM-dd";
        dType ="D";
    }
    else  //默认取分钟数据
    {
        dtFormat = "yyyy-MM-dd hh:mm";
        dType ="M";
    }

    QList< QList<QString> > rows;
    QString sql = "SELECT COUNT(timestamp) FROM TStatisticalData";
    sql += " WHERE timestamp<='"+timeEnd.toString(dtFormat)+"' and timestamp>='"+timeStart.toString(dtFormat)+"'";
    sql += " AND code='"+factorCode+"' AND dataType='"+dType+"'";

    bool r = bSqlite.find(sql, &rows);
    if (r==false || rows.count()==0)
        return 0;
    return rows.at(0).at(0).toInt();
}

BHistoryData::BHistoryData()
{

}

BHistoryData::BHistoryData(QString tableName)
{
    this->tableName = tableName;
}

BHistoryData::~BHistoryData()
{

}

bool BHistoryData::createTable()
{
    if (this->tableName.length()==0)
        return false;
    else
        return createTable(this->tableName);
}

bool BHistoryData::createTable(QString tableName)
{
    bool r = true;
    //(1)判断数据表是否存在。
    if (bSqlite.isTableExist(tableName))
    {
       r = r && deleteTable(tableName);

       //qDebug()<<__FUNCTION__<<": delete table: "<<tableName<<"# result = "<<r<<endl;
       //return true;   //数据表存在就不创建了
    }

    //（2）创建数据表
    QMap<QString,QString> table_data;
    table_data.insert("dataFlag","TEXT DEFAULT 'N'");
    table_data.insert("value","DOUBLE DEFAULT 0");
    table_data.insert("timestamp","DATETIME DEFAULT '2021-11-01 00:00:00:000'");
    table_data.insert("id","INTEGER UNIQUE");     
    r = r && bSqlite.create_table(tableName,table_data);

    //qDebug()<<__FUNCTION__<<": create table: "<<tableName<<"# result = "<<r<<endl;

    if (r==false)
        return r;
/*
    //（2）采用事务的方式快速写入批量记录
    SysDictionary sysDic;
    //int recCount = sysDic.getSTORAGE_TIME()*60*60/sysDic.getINTERVAL();
    int recCount = sysDic.getSTORAGE_TIME()*60*60/5;  //按照 5 秒的时间间隔采集数据

    sysDic.putTABLE_LENGTH(recCount);

    QList<QString> sqlList;
    for (int i=1;i<=recCount;i++)
    {
        sqlList.append("INSERT INTO '"+ tableName+"' ( id ) VALUES ("+QString::number(i)+")");
    }
    r = bSqlite.execTransaction(sqlList);

    //（3）创建索引
    table_data.clear();
    table_data.insert("id", "ASC");
    bSqlite.create_index("Idx_"+tableName,tableName,table_data);
*/
    //(4) 初始化数据的读写和计算相关指针
    QDateTime dt=QDateTime::currentDateTime();
    table_data.clear();
    table_data.insert("tablename", "'"+tableName+"'");
    table_data.insert("write_ptr","1");
    table_data.insert("upload_ptr","'"+dt.toString("yyyy-MM-dd HH:mm:ss:zzz")+"'");
    table_data.insert("CalcTimestampM","'"+dt.toString("yyyy-MM-dd HH:mm")+"'");
    table_data.insert("CalcTimestampH","'"+dt.toString("yyyy-MM-dd HH")+"'");
    table_data.insert("CalcTimestampD","'"+dt.toString("yyyy-MM-dd")+"'");
    r = r && bSqlite.add("TFactorDataList",table_data);

    //qDebug()<<__FUNCTION__<<": add factor recorder: "<<tableName<<"# result = "<<r<<endl;

    return r;
}

bool BHistoryData::deleteTable(QString tableName)
{
    bool r = bSqlite.delete_table(tableName);  //删除数据表
    return r && bSqlite.del("TFactorDataList","tablename='"+tableName+"'");  //删除表名列表中的记录
}

bool BHistoryData::initDataTables()
{
    //删除现有数据表
    QList<QString> tablesList = getDataTablesList();
    for (int i=0;i<tablesList.count();i++)
    {
        deleteTable(tablesList.at(i));
    }

    //根据监测因子列表，创建数据表
    for (int i=0;i<BComArgs::ComArgsList.count();i++)
    {
         MComArgs * mca = BComArgs::ComArgsList.at(i);
         if(mca->Status!=UNUSED)
         {
             for (int j=0;j<mca->ComFactorArgsList.count();j++)
             {
                 MFactor factor =mca->ComFactorArgsList.at(j);
                 QString tableName = factor.dataPortName+"_"+factor.code;
                 createTable(tableName);
             }
         }
    }

}

bool BHistoryData::initDataTables(QString portName)
{
    bool r=true;
    QList<QString> tablesList = getDataTablesList(portName);
    for (int i=0;i<tablesList.count();i++)
    {
        r = r&& deleteTable(tablesList.at(i));

        //qDebug()<<__FUNCTION__<<": delete table: "<<tablesList.at(i)<<endl;
    }

    BComArgs bca;
    MComArgs mca;
    bca.getArgsByPortname(portName,mca);

    //根据监测因子列表，创建数据表
     if(mca.Status!=UNUSED)
     {
         for (int j=0;j<mca.ComFactorArgsList.count();j++)
         {
             MFactor factor =mca.ComFactorArgsList.at(j);

             if (factor.flag!=0) //不是监测的数据因子，不用创建本地数据表
                 continue;

             QString tableName;
             if (factor.deviceId.trimmed().length()>0)
             {
                 tableName=factor.dataPortName+"_"+factor.deviceId.trimmed()+"_"+factor.code;
                 tableName=tableName.replace("xx",factor.deviceId.trimmed());
             }
             else
                 tableName=factor.dataPortName+"_"+factor.code;
             r = r&& createTable(tableName);

             //qDebug()<<__FUNCTION__<<": create table: "<<tableName<<endl;
         }
     }

    return r;
}

//删除超出存储时长的数据
bool BHistoryData::packDataTables()
{
    bool r=true;
    SysDictionary sysDic;
    long storageTime=sysDic.getSTORAGE_TIME()*60*60;
    QString dtime = QDateTime::currentDateTime().addSecs(-storageTime).toString("yyyy-MM-dd HH:mm:ss:zzz");
    QList<QString> tableList = getDataTablesList();
    for (int i=0;i<tableList.count();i++)
    {
       QString where = "timestamp<'"+dtime+"'";
       r = r && bSqlite.del(tableList.at(i),where);
    }
    return r;
}

QList<QString> BHistoryData::getDataTablesList()
{
    QList<QString> tablesList;
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("tablename");
    where.insert("1","1");

    bSqlite.find("TFactorDataList", key, where, &rows);

    for (int i=0;i<rows.count();i++)
    {
        tablesList.append(rows.at(i).at(0));
    }
    return tablesList;
}

QList<QString> BHistoryData::getDataTablesList(QString portName)
{
    QList<QString> tablesList;
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("tablename");
    where.insert("substr(tablename,1,"+QString::number(portName.length())+")","'"+portName+"'");

    bSqlite.find("TFactorDataList", key, where, &rows);
    for (int i=0;i<rows.count();i++)
    {
        tablesList.append(rows.at(i).at(0));
    }
    return tablesList;
}

bool BHistoryData::writeToDB(MFactor *factor)
{
    //增加的方式写入记录
    QMap<QString,QString> data;
    int in,dn;
    FactorDataType::getDataLength(factor->nType,in,dn);
    if (dn==0)
        data.insert("value",QString::number((int)(factor->value+0.5)));
    else
        data.insert("value",QString::number(factor->value,'f',dn));
    data.insert("dataFlag","'"+factor->dataFlag+"'");
    data.insert("timestamp","'"+factor->valueTimestamp.toString("yyyy-MM-dd hh:mm:ss:zzz")+"'");
    bSqlite.add(factor->localTableName,data);

 /*
    //更新的方式写入记录
    //获取数据表的写指针
    int writePtr=this->getTableWritePtr(factor->localTableName);
    if (writePtr==-1)
        return false;

    int in,dn;
    DataType::getDataLength(factor->nType,in,dn);

    //写入数据
    QMap<QString,QString> where, data;
    where.insert("id",QString::number(writePtr));
    if (dn==0)
        data.insert("value",QString::number((int)(factor->value+0.5)));
    else
        data.insert("value",QString::number(factor->value,'f',dn));
    data.insert("dataFlag","'"+factor->dataFlag+"'");
    data.insert("timestamp","'"+factor->valueTimestamp.toString("yyyy-MM-dd hh:mm:ss:zzz")+"'");
    bSqlite.update(factor->localTableName,where,data);

    //更新写指针
    writePtr ++;
    SysDictionary sysDic;
    int tableLength = sysDic.getTABLE_LENGTH();

    writePtr = writePtr > (tableLength) ? 1 : writePtr;

    return putTableWritePtr(factor->localTableName,writePtr,"'"+factor->valueTimestamp.toString("yyyy-MM-dd hh:mm:ss:zzz")+"'");
*/
}

int BHistoryData::getTableWritePtr(QString tableName)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("write_ptr");
    where.insert("tablename","'"+tableName+"'");
    bSqlite.find("TFactorDataList", key, where, &rows);
    if (rows.count()==0)
        return  -1;
    else
        return rows.at(0).at(0).toInt();
}

bool BHistoryData::initCalcTimestamp()
{
    QString tt;
    for (int i=0;i<BComArgs::ComArgsList.count();i++)
    {
         MComArgs * mca = BComArgs::ComArgsList.at(i);
         if(mca->Status!=UNUSED)
         {
             for (int j=0;j<mca->ComFactorArgsList.count();j++)
             {
                 MFactor factor = mca->ComFactorArgsList.at(j);
                 bool rr = getTableCalcTimestampM(factor.localTableName, tt);
                 if (rr && QDateTime::fromString(tt,"yyyy-MM-dd HH:mm").addDays(3)<QDateTime::currentDateTime())
                 {
                     putTableCalcTimestampM(factor.localTableName,QDateTime::currentDateTime().addDays(-3).toString("yyyy-MM-dd HH:mm"));
                     putTableCalcTimestampH(factor.localTableName,QDateTime::currentDateTime().addDays(-3).toString("yyyy-MM-dd HH"));
                 }
                 rr = getTableCalcTimestampD(factor.localTableName, tt);
                 if (rr && QDateTime::fromString(tt,"yyyy-MM-dd").addDays(30)<QDateTime::currentDateTime())
                 {
                    putTableCalcTimestampD(factor.localTableName,QDateTime::currentDateTime().addDays(-30).toString("yyyy-MM-dd"));
                 }
             }
         }
    }
    return true;
}

bool BHistoryData::getTableCalcTimestampM(QString tableName, QString &timestamp)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("CalcTimestampM");
    where.insert("tablename","'"+tableName+"'");
    bSqlite.find("TFactorDataList", key, where, &rows);
    if (rows.count()==0)
        return  false;
    else
    {
        timestamp = rows.at(0).at(0);
        return true;
    }
}

bool BHistoryData::getTableCalcTimestampH(QString tableName, QString &timestamp)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("CalcTimestampH");
    where.insert("tablename","'"+tableName+"'");
    bSqlite.find("TFactorDataList", key, where, &rows);
    if (rows.count()==0)
        return  false;
    else
    {
        timestamp = rows.at(0).at(0);
        return true;
    }
}

bool BHistoryData::getTableCalcTimestampD(QString tableName, QString &timestamp)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("CalcTimestampD");
    where.insert("tablename","'"+tableName+"'");
    bSqlite.find("TFactorDataList", key, where, &rows);
    if (rows.count()==0)
        return  false;
    else
    {
        timestamp = rows.at(0).at(0);
        return true;
    }
}

bool BHistoryData::getTableCalcTimestamp(QString tableName, QString &timestampM, QString &timestampH, QString &timestampD)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("CalcTimestampM");
    key.append("CalcTimestampH");
    key.append("CalcTimestampD");
    where.insert("tablename","'"+tableName+"'");
    bSqlite.find("TFactorDataList", key, where, &rows);
    if (rows.count()==0)
        return  false;
    else
    {
        timestampM = rows.at(0).at(0);
        timestampH = rows.at(0).at(1);
        timestampD = rows.at(0).at(2);
        return true;
    }
}

bool BHistoryData::getTablePtr(QString tableName, int &write_ptr, QString &upload_ptr)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("write_ptr");
    key.append("upload_ptr");
    where.insert("tablename","'"+tableName+"'");
    bSqlite.find("TFactorDataList", key, where, &rows);
    if (rows.count()==0)
        return  false;
    else
    {
        write_ptr = rows.at(0).at(0).toInt();
        upload_ptr = rows.at(0).at(1);
        return true;
    }
}

bool BHistoryData::putTableWritePtr(QString tableName, int writePtr)
{
    QMap<QString,QString> where, data;
    where.insert("tablename","'"+tableName+"'");
    data.insert("write_ptr",QString::number(writePtr));
    return bSqlite.update("TFactorDataList",where,data);
}

bool BHistoryData::putTableWritePtr(QString tableName, int writePtr, QString timestamp)
{
    QMap<QString,QString> where, data;
    where.insert("tablename","'"+tableName+"'");
    data.insert("write_ptr",QString::number(writePtr));
    data.insert("timestamp",timestamp);
    return bSqlite.update("TFactorDataList",where,data);
}

bool BHistoryData::putTableUploadPtr(QString tableName, QString uploadPtr)
{
    QMap<QString,QString> where, data;
    where.insert("tablename","'"+tableName+"'");
    data.insert("upload_ptr","'"+uploadPtr+"'");
    return bSqlite.update("TFactorDataList",where,data);
}

bool BHistoryData::putTableCalcTimestampM(QString tableName, QString timestamp)
{
    QMap<QString,QString> where, data;
    where.insert("tablename","'"+tableName+"'");
    data.insert("CalcTimestampM","'"+timestamp+"'");
    return bSqlite.update("TFactorDataList",where,data);
}

bool BHistoryData::putTableCalcTimestampH(QString tableName, QString timestamp)
{
    QMap<QString,QString> where, data;
    where.insert("tablename","'"+tableName+"'");
    data.insert("CalcTimestampH","'"+timestamp+"'");
    return bSqlite.update("TFactorDataList",where,data);
}

bool BHistoryData::putTableCalcTimestampD(QString tableName, QString timestamp)
{
    QMap<QString,QString> where, data;
    where.insert("tablename","'"+tableName+"'");
    data.insert("CalcTimestampD","'"+timestamp+"'");
    return bSqlite.update("TFactorDataList",where,data);
}

bool BHistoryData::putTableCalcTimestamp(QString tableName, QString &timestampM, QString &timestampH, QString &timestampD)
{
    QMap<QString,QString> where, data;
    where.insert("tablename","'"+tableName+"'");
    data.insert("CalcTimestampM","'"+timestampM+"'");
    data.insert("CalcTimestampH","'"+timestampH+"'");
    data.insert("CalcTimestampD","'"+timestampD+"'");
    return bSqlite.update("TFactorDataList",where,data);
}

bool BHistoryData::getTimestampById(QString tableName, int id, QDateTime &datetime)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("timestamp");
    where.insert("id",QString::number(id));
    bool r = bSqlite.find(tableName,key,where,&rows);
    if (r==false || rows.count()==0)
        return false;
    datetime = QDateTime::fromString(rows.at(0).at(0),"yyyy-MM-dd hh:mm:ss:zzz");
    return true;

}

//获取未上传的实时数据列表
bool BHistoryData::getSerialData(MFactor *factor, QList<RealTimeData> & data)
{
    //获取数据表的写指针和数据上传指针

    int writePtr;
    QString uploadPtr;
    this->getTablePtr(factor->localTableName, writePtr, uploadPtr);     

    SysDictionary sysDic;
    int st = sysDic.getSTORAGE_TIME();
    int inv = sysDic.getINTERVAL();

    QDateTime nowT = QDateTime::currentDateTime();
    //若实时数据未上传的时间超过2个上传时间间隔，就取当前时间前2个上传时间间隔范围内的数据
    if ( QDateTime::fromString(uploadPtr,"yyyy-MM-dd HH:mm:ss:zzz").addSecs(inv*2)<nowT)
        uploadPtr=nowT.addSecs(-(inv*2)).toString("yyyy-MM-dd HH:mm:ss:zzz");

    QList<QString> key;
    QMap<QString,QString> where;
    QString orderby;
    QList< QList<QString> > rows;
    key.append("value");
    key.append("substr(timestamp,1,19) as t");
    key.append("dataFlag");
    where.insert("true","((timestamp<='"+nowT.toString("yyyy-MM-dd hh:mm:ss:zzz")+"' and timestamp>'"+uploadPtr+"'))");
    orderby = "t asc";

    bool r = bSqlite.find(factor->localTableName, key, where, orderby, &rows);
    if (r==false || rows.count()==0)
        return false;

    QDateTime dt;
    for (int i=0;i<rows.count();i++)
    {
        RealTimeData d;
        d.code = factor->code;
        d.timestamp = rows.at(i).at(1);
        d.value = rows.at(i).at(0).toDouble();
        d.dataFlag = rows.at(i).at(2);
        data.append(d);
    }
    return true;
}

bool BHistoryData::getSerialData(MFactor *factor, QVector<double> &values, QVector<double> &timestamps)
{
    #define  DataMaxLength 120
    #define  MagicTime "2021-11-01 00:00:00:000"
    //获取数据表的写指针
    int writePtr=this->getTableWritePtr(factor->localTableName);
    if (writePtr==-1)
        return false;

    QList<QString> key;
    QMap<QString,QString> where;
    QString orderby;
    QList< QList<QString> > rows;
    key.append("value");
    key.append("timestamp");
    if (writePtr>=DataMaxLength)
    {
        where.insert("true","((id<"+QString::number(writePtr)+" and id>="+QString::number(writePtr-DataMaxLength)+") and (timestamp!='"+MagicTime+"'))");
    }
    else
    {
        SysDictionary sysDic;
        int h = sysDic.getSTORAGE_TIME();
        int v = sysDic.getINTERVAL();
        where.insert("true","((id<="+QString::number(writePtr)+" or id>="+ QString::number(h*60*60/v-(DataMaxLength-writePtr)) +") and (timestamp!='"+MagicTime+"'))");
    }
    orderby = "timestamp asc";
    bool r = bSqlite.find(factor->localTableName, key, where, orderby, &rows);

    if (r==false || rows.count()==0)
        return false;

    QDateTime dt;
    for (int i=0;i<rows.count();i++)
    {
        values.append(rows.at(i).at(0).toDouble());        
        dt = QDateTime::fromString(rows.at(i).at(1),"yyyy-MM-dd hh:mm:ss:zzz");
        timestamps.append(dt.toMSecsSinceEpoch());
    }
    return true;
}

bool BHistoryData::getSerialData(QString tableName, QDateTime timeStart, QDateTime timeEnd, QVector<double> &values, QVector<double> &timestamps)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QString orderby;
    QList< QList<QString> > rows;
    key.append("value");
    key.append("timestamp");

    where.insert("true","(timestamp<='"+timeEnd.toString("yyyy-MM-dd hh:mm:ss:zzz")+"' and timestamp>='"+timeStart.toString("yyyy-MM-dd hh:mm:ss:zzz")+"')");

    orderby = "timestamp asc";

    bool r = bSqlite.find(tableName, key, where, orderby, &rows);

    if (r==false || rows.count()==0)
        return false;

    QDateTime dt;
    for (int i=0;i<rows.count();i++)
    {
        values.append(rows.at(i).at(0).toDouble());
        dt = QDateTime::fromString(rows.at(i).at(1),"yyyy-MM-dd hh:mm:ss:zzz");
        timestamps.append(dt.toMSecsSinceEpoch());
    }
    return true;
}

bool BHistoryData::getSerialData(QString tableName, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                                 QVector<double> &values, QVector<double> &timestamps)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QString orderby;
    QList< QList<QString> > rows;
    key.append("value");
    key.append("timestamp");

    where.insert("true","(timestamp<='"+timeEnd.toString("yyyy-MM-dd hh:mm:ss:zzz")+"' and timestamp>='"+timeStart.toString("yyyy-MM-dd hh:mm:ss:zzz")+"')");

    orderby = "timestamp asc limit "+ QString::number(limit)+" offset "+QString::number(offset);

    bool r = bSqlite.find(tableName, key, where, orderby, &rows);

    if (r==false || rows.count()==0)
        return false;

    QDateTime dt;
    for (int i=0;i<rows.count();i++)
    {
        values.append(rows.at(i).at(0).toDouble());
        dt = QDateTime::fromString(rows.at(i).at(1),"yyyy-MM-dd hh:mm:ss:zzz");
        timestamps.append(dt.toMSecsSinceEpoch());
    }
    return true;
}

bool BHistoryData::getSerialData(QString factorCode, QDateTime timeStart, QDateTime timeEnd, double max, double min, double avg)
{
    BFactor bf;
    MFactor factor;
    bf.getFactorInfoByCode(factorCode, factor);
    QList< QList<QString> > rows;
    QString sql = "SELECT MAX(value) as vmax, MIN(value) as vmin, AVG(value) as vavg FROM '"+factor.localTableName+"'";
    sql += " WHERE timestamp<='"+timeEnd.toString("yyyy-MM-dd hh:mm:ss:zzz")+"' and timestamp>='"+timeStart.toString("yyyy-MM-dd hh:mm:ss:zzz")+"'";

    bool r = bSqlite.find(sql, &rows);

    if (r==false || rows.count()==0)
        return 0;

    QDateTime dt;
    max = rows.at(0).at(0).toDouble();
    min = rows.at(0).at(1).toDouble();
    avg = rows.at(0).at(2).toDouble();
    return true;
}

bool BHistoryData::getSerialDataM(QString factorCode, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                                  QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                                  QVector<double> &timestamps)
{
    return getHistoryData(factorCode, "M", timeStart, timeEnd, limit, offset,
                          valuesMax, valuesMin, valuesAvg, timestamps);
}

bool BHistoryData::getSerialDataM(QString factorCode, QDateTime timeStart, QDateTime timeEnd,
                                  QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                                  QVector<double> &timestamps)

{
    return getHistoryData(factorCode, "M", timeStart, timeEnd, valuesMax, valuesMin, valuesAvg, timestamps);
}


bool BHistoryData::getSerialDataM(QString factorCode, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data)
{
    return getHistoryData(factorCode,"M",timeStart,timeEnd,data);
}

bool BHistoryData::getSerialDataH(QString factorCode, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                                  QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                                  QVector<double> &timestamps)
{
    return getHistoryData(factorCode, "H", timeStart, timeEnd, limit, offset,
                          valuesMax, valuesMin, valuesAvg, timestamps);
}

bool BHistoryData::getSerialDataH(QString factorCode, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data)
{
    return getHistoryData(factorCode,"H",timeStart,timeEnd,data);
}

bool BHistoryData::getSerialDataD(QString factorCode, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                                  QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                                  QVector<double> &timestamps)
{
    return getHistoryData(factorCode, "D", timeStart, timeEnd, limit, offset,
                          valuesMax, valuesMin, valuesAvg, timestamps);
}

bool BHistoryData::getSerialDataD(QString factorCode, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data)
{
    return getHistoryData(factorCode,"D",timeStart,timeEnd,data);
}

int BHistoryData::getRecordCount(QString tableName, QDateTime timeStart, QDateTime timeEnd)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QString orderby;
    QList< QList<QString> > rows;
    key.append("count(value) as c");

    where.insert("true","(timestamp<='"+timeEnd.toString("yyyy-MM-dd hh:mm:ss:zzz")+"' and timestamp>='"+timeStart.toString("yyyy-MM-dd hh:mm:ss:zzz")+"')");

    orderby = "timestamp asc";

    bool r = bSqlite.find(tableName, key, where, orderby, &rows);

    if (r==false || rows.count()==0)
        return 0;

    return rows.at(0).at(0).toInt();
}

int BHistoryData::getRecordCountM(QString factorCode, QDateTime timeStart, QDateTime timeEnd)
{
    return getHistoryDataLength(factorCode, "M", timeStart, timeEnd);
}

int BHistoryData::getRecordCountH(QString factorCode, QDateTime timeStart, QDateTime timeEnd)
{
    return getHistoryDataLength(factorCode, "H", timeStart, timeEnd);
}

int BHistoryData::getRecordCountD(QString factorCode, QDateTime timeStart, QDateTime timeEnd)
{
    return getHistoryDataLength(factorCode, "D", timeStart, timeEnd);
}

