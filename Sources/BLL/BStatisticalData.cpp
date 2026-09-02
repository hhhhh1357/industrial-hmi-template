#include "Headers/BLL/BStatisticalData.h"

#include <QDateTime>

#include <Headers/Model/MFactor.h>

#include <Headers/BLL/BFactor.h>

#include <Sources/Tools/datatype.h>

MStatisticalData data_01001,data_01018,data_01019,data_21001,data_21003,data_21011;
int iTimes_01001=0,iTimes_01018=0,iTimes_01019=0,iTimes_21001=0,iTimes_21003=0,iTimes_21011=0;

bool BStatisticalData::readData(QString factor, QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    QList< QList<QString> > rows;
    QString sql = "SELECT * FROM 'TStatisticalData'";
    sql += " WHERE timestamp<='"+timeEnd+"' and timestamp>='"+timeStart+"'"; //指定时间范围
    sql += " and code='"+factor+"'";  //指定因子
    sql += " and dataType='"+dataType+"'";  //指定类型数据
    sql += " ORDER BY timestamp";

    bool r = bSqlite.find(sql, &rows);

    if (r==false || rows.count()==0)
        return false;

    foreach (QList<QString> d, rows)
    {
        MStatisticalData msd;
        msd.id=d.at(0);
        msd.code=d.at(1);
        msd.dataType=d.at(2);
        msd.timestamp=d.at(3);
        msd.valueCou=d.at(4).toDouble();
        msd.valueMax=d.at(5).toDouble();
        msd.valueMin=d.at(6).toDouble();
        msd.valueAvg=d.at(7).toDouble();
        msd.dataFlag=d.at(8);
        msd.flag=d.at(9).toInt();
        data.append(msd);
    }
    return r;
}

bool BStatisticalData::readData(QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    QList< QList<QString> > rows;
    QString sql = "SELECT * FROM 'TStatisticalData'";
    sql += " WHERE timestamp<='"+timeEnd+"' and timestamp>='"+timeStart+"'"; //指定时间范围
    sql += " and dataType='"+dataType+"'";  //指定类型数据
    sql += " ORDER BY timestamp";

    bool r = bSqlite.find(sql, &rows);

    if (r==false || rows.count()==0)
        return false;

    foreach (QList<QString> d, rows)
    {
        MStatisticalData msd;
        msd.id=d.at(0);
        msd.code=d.at(1);
        msd.dataType=d.at(2);
        msd.timestamp=d.at(3);
        msd.valueCou=d.at(4).toDouble();
        msd.valueMax=d.at(5).toDouble();
        msd.valueMin=d.at(6).toDouble();
        msd.valueAvg=d.at(7).toDouble();
        msd.dataFlag=d.at(8);
        msd.flag=d.at(9).toInt();
        data.append(msd);
    }
    return r;
}

bool BStatisticalData::outData(QString factor, QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    QList< QList<QString> > rows;
    QString sql = "SELECT * FROM 'TStatisticalData'";
    sql += " WHERE timestamp<='"+timeEnd+"' and timestamp>='"+timeStart+"'"; //指定时间范围
    sql += " and code='"+factor+"'";  //指定因子
    sql += " and dataType='"+dataType+"'";  //指定类型数据
    sql += " and flag=0";
    sql += " ORDER BY timestamp";

    bool r = bSqlite.find(sql, &rows);

    if (r==false || rows.count()==0)
        return false;

    foreach (QList<QString> dd, rows)
    {
        MStatisticalData msd;
        msd.id=dd.at(0);
        msd.code=dd.at(1);
        msd.dataType=dd.at(2);
        msd.timestamp=dd.at(3);
        msd.valueCou=dd.at(4).toDouble();
        msd.valueMax=dd.at(5).toDouble();
        msd.valueMin=dd.at(6).toDouble();
        msd.valueAvg=dd.at(7).toDouble();
        msd.dataFlag=dd.at(8);
        msd.flag=dd.at(9).toInt();
        data.append(msd);
    }

    if (r)  //将数据上传标识 flag 值置为 1
    {
        QMap<QString,QString> where, data;
        QString wv = "(timestamp<='"+timeEnd+"' and timestamp>='"+timeStart+"'";
        wv += " and code='"+factor+"'";  //指定因子
        wv += " and dataType='"+dataType+"'";  //指定类型数据
        sql += " and flag=0 )";
        where.insert("true", wv);
        data.insert("flag","1");
        r = bSqlite.update("TStatisticalData",where,data);
    }
    return r;
}

bool BStatisticalData::outData(QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    QList< QList<QString> > rows;
    BSysConfig sysConfig;

    bool b_01001=false,b_01018=false,b_01019=false,b_21001=false,b_21003=false,b_21011=false;
    QString sql = "SELECT * FROM 'TStatisticalData'";
    sql += " WHERE timestamp<='"+timeEnd+"' and timestamp>='"+timeStart+"'"; //指定时间范围
    sql += " and dataType='"+dataType+"'";  //指定类型数据
    sql += " and flag=0";
    sql += " ORDER BY timestamp";
//qDebug()<<"SQL: "<<sql<<endl;;
    bool r = bSqlite.find(sql, &rows);

    //////////
    QString AllTable;
    // 执行查询以获取所有表名
        QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table'", bSqlite.db);
        while (query.next())
        {
            QString tableName = query.value(0).toString();
            AllTable += tableName;
            AllTable += ",";
        }
    //////////////


    if (r==false || rows.count()==0)
        return false;

qDebug()<<"----rows.count()= "<<rows.count()<<endl;
    foreach (QList<QString> dd, rows)
    {
        MStatisticalData msd;
        msd.id=dd.at(0);
        msd.code=dd.at(1);
        msd.dataType=dd.at(2);
        msd.timestamp=dd.at(3);
        msd.valueCou=dd.at(4).toDouble();
        msd.valueMax=dd.at(5).toDouble();
        msd.valueMin=dd.at(6).toDouble();
        msd.valueAvg=dd.at(7).toDouble();
        msd.dataFlag=dd.at(8);
        msd.flag=dd.at(9).toInt();
        data.append(msd);

        if(msd.code.indexOf("w01001")>= 0)
            data_01001 = msd;
        if(msd.code.indexOf("w01018")>= 0)
            data_01018 = msd;
        if(msd.code.indexOf("w01019")>= 0)
            data_01019 = msd;
        if(msd.code.indexOf("w21001")>= 0)
            data_21001 = msd;
        if(msd.code.indexOf("w21003")>= 0)
            data_21003 = msd;
        if(msd.code.indexOf("w21011")>= 0)
            data_21011 = msd;
    }

    for(int i =0;i<data.count();i++)
    {
        if(data[i].code.indexOf("w01001")>= 0)
        {
            b_01001 = true;
            iTimes_01001 = 0;
            break;
        }
    }
    //qDebug()<<"----b_01001: "<<b_01001<<endl;
    //qDebug()<<"----data_01001.code: "<<data_01001.code<<endl;
    //qDebug()<<"----data_01001.valueAvg: "<<data_01001.valueAvg<<endl;
    if((!b_01001)&&(iTimes_01001 < (7200/sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))&&(AllTable.indexOf("w01001")>=0))
    {
        iTimes_01001++;
        data.append(data_01001);
    }
    //qDebug()<<"----data.count()= "<<data.count()<<endl;

    for(int i =0;i<data.count();i++)
    {
        if(data[i].code.indexOf("w01018")>= 0)
        {
            b_01018 = true;
            iTimes_01018 = 0;
            break;
        }
    }
    if((!b_01018)&&(iTimes_01018 < (7200/sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))&&(AllTable.indexOf("w01018")>=0))
    {
        iTimes_01018++;
        data.append(data_01018);
    }

    for(int i =0;i<data.count();i++)
    {
        if(data[i].code.indexOf("w01019")>= 0)
        {
            b_01019 = true;
            iTimes_01019 = 0;
            break;
        }
    }
    if((!b_01019)&&(iTimes_01019 < (7200/sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))&&(AllTable.indexOf("w01019")>=0))
    {
        iTimes_01019++;
        data.append(data_01019);
    }

    for(int i =0;i<data.count();i++)
    {
        if(data[i].code.indexOf("w21001")>= 0)
        {
            b_21001 = true;
            iTimes_21001 = 0;
            break;
        }
    }
    if((!b_21001)&&(iTimes_21001 < (7200/sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))&&(AllTable.indexOf("w21001")>=0))
    {
        iTimes_21001++;
        data.append(data_21001);
    }

    for(int i =0;i<data.count();i++)
    {
        if(data[i].code.indexOf("w21003")>= 0)
        {
            b_21003 = true;
            iTimes_21003 = 0;
            break;
        }
    }
    if((!b_21003)&&(iTimes_21003 < (7200/sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))&&(AllTable.indexOf("w21003")>=0))
    {
        iTimes_21003++;
        data.append(data_21003);
    }

    for(int i =0;i<data.count();i++)
    {
        if(data[i].code.indexOf("w21011")>= 0)
        {
            b_21011 = true;
            iTimes_21011 = 0;
            break;
        }
    }
    if((!b_21011)&&(iTimes_21011 < (7200/sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))&&(AllTable.indexOf("w21011")>=0))
    {
        iTimes_21011++;
        data.append(data_21011);
    }

    if (r)  //将数据上传标识 flag 值置为 1
    {
        QMap<QString,QString> where, data;
        QString wv = "(timestamp<='"+timeEnd+"' and timestamp>='"+timeStart+"'";
        wv += " and dataType='"+dataType+"')";  //指定类型数据
        sql += " and flag=0 )";
        where.insert("true", wv);
        data.insert("flag","1");
        r = bSqlite.update("TStatisticalData",where,data);
    }
    return r;
}

bool BStatisticalData::writeToDB(MStatisticalData &data)
{
    MFactor factor;
    BFactor bf;

    //先查一下数据是否存在

    QList<MStatisticalData> dlist;
    dlist.clear();
    if (data.dataType == "M")
        readMinuteData(data.code,data.timestamp,data.timestamp,dlist);
    else if (data.dataType == "H")
        readHourData(data.code,data.timestamp,data.timestamp,dlist);
    else if (data.dataType == "D")
        readDayData(data.code,data.timestamp,data.timestamp,dlist);
    if (dlist.count()>0)
    {
        return false;
    }

    bf.findFactor(data.code,factor);

    int in,dn;
    FactorDataType::getDataLength(factor.nType,in,dn);

    QMap<QString,QString> d;
    d.insert("id","'"+QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")+"'");
    d.insert("code","'"+data.code+"'");
    d.insert("dataType","'"+data.dataType+"'");
    d.insert("timestamp","'"+data.timestamp+"'");
    if (dn==0)
    {
        d.insert("valueCou",QString::number((int)(data.valueCou+0.5)));
        d.insert("valueMax",QString::number((int)(data.valueMax+0.5)));
        d.insert("valueMin",QString::number((int)(data.valueMin+0.5)));
        d.insert("valueAvg",QString::number((int)(data.valueAvg+0.5)));
    }
    else
    {
        d.insert("valueCou",QString::number(data.valueCou,'f',dn));
        d.insert("valueMax",QString::number(data.valueMax,'f',dn));
        d.insert("valueMin",QString::number(data.valueMin,'f',dn));
        d.insert("valueAvg",QString::number(data.valueAvg,'f',dn));
    }
    d.insert("dataFlag","'"+data.dataFlag+"'");
    d.insert("flag","0");  //数据上传标识值为 0
    return bSqlite.add("TStatisticalData",d);
}

bool BStatisticalData::readMinuteData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
   return readData(factor, "M", timeStart, timeEnd, data);
}

bool BStatisticalData::readMinuteData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
   return readData("M", timeStart, timeEnd, data);
}

bool BStatisticalData::outMinuteData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return outData(factor, "M", timeStart, timeEnd, data);
}

bool BStatisticalData::outMinuteData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return outData("M", timeStart, timeEnd, data);
}

bool BStatisticalData::outMinuteData(QList<MStatisticalData> &data)
{
    BSysConfig sysConfig;
    QDateTime dt = QDateTime::currentDateTime();
    return outData("M", dt.addSecs(-5*sysConfig.SysDic.getDATA_UPLOAD_MINUTE()).toString("yyyy-MM-dd HH:mm"), dt.toString("yyyy-MM-dd HH:mm"), data);
}

bool BStatisticalData::readHourData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return readData(factor, "H", timeStart, timeEnd, data);
}

bool BStatisticalData::readHourData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return readData("H", timeStart, timeEnd, data);
}

bool BStatisticalData::outHourData(QList<MStatisticalData> &data)
{
    BSysConfig sysConfig;
    QDateTime dt = QDateTime::currentDateTime();
    return outData("H", dt.addSecs(-3*sysConfig.SysDic.getDATA_UPLOAD_HOUR()).toString("yyyy-MM-dd HH:mm"), dt.toString("yyyy-MM-dd HH:mm"), data);
}

bool BStatisticalData::outHourData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return outData(factor, "H", timeStart, timeEnd, data);
}

bool BStatisticalData::outHourData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return outData("H", timeStart, timeEnd, data);
}

bool BStatisticalData::readDayData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return readData(factor, "D", timeStart, timeEnd, data);
}

bool BStatisticalData::readDayData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return readData("D", timeStart, timeEnd, data);
}

bool BStatisticalData::outDayData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return outData(factor, "D", timeStart, timeEnd, data);
}

bool BStatisticalData::outDayData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data)
{
    return outData("D", timeStart, timeEnd, data);
}

bool BStatisticalData::outDayData(QList<MStatisticalData> &data)
{
    QDateTime dt = QDateTime::currentDateTime();
    return outData("D", dt.addDays(-2).toString("yyyy-MM-dd HH:mm"), dt.toString("yyyy-MM-dd HH:mm"), data);
}

bool BStatisticalData::packDB()
{
    int mDays=3; //分钟级数据保留的天数
    int hDays=10; //小时级数据保留的天数
    int dDays=30; //日数据保留的天数

    QDateTime nowDate = QDateTime::currentDateTime();
    nowDate = QDateTime::fromString(nowDate.toString("yyyyMMdd")+"000000","yyyyMMddHHmmss");
    //删除分钟级数据
    bool r1 = bSqlite.del("TStatisticalData","dataType='M' and timestamp<='"+nowDate.addDays(-mDays).toString("yyyyMMddHHmmss")+"'");

    //删除小时级数据
    bool r2 = bSqlite.del("TStatisticalData","dataType='H' and timestamp<='"+nowDate.addDays(-hDays).toString("yyyyMMddHHmmss")+"'");

    //删除日数据
    bool r3 = bSqlite.del("TStatisticalData","dataType='D' and timestamp<='"+nowDate.addDays(-dDays).toString("yyyyMMddHHmmss")+"'");

    return r1&&r2&&r3;
}
