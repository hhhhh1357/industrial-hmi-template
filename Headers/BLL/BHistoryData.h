#ifndef BHISTORYDATA_H
#define BHISTORYDATA_H

#include <QString>
#include <QMap>
#include <QMutexLocker>

#include "Headers/DAL/dal.h"
#include "Headers/Model/MFactor.h"
#include "Headers/Model/MStatisticalData.h"

extern Sqlite bSqlite;
extern QMutex mutexSqlite;

class RealTimeData
{
public:
    QString code;
    QString timestamp;
    double value;
    QString dataFlag;
};

class BHistoryData
{
private:
    bool getHistoryData(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                        QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                        QVector<double> &timestamps);
    bool getHistoryData(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd,
                        QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                        QVector<double> &timestamps);
    bool getHistoryData(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data);
    int getHistoryDataLength(QString factorCode, QString dataType, QDateTime timeStart, QDateTime timeEnd);

public:
    QString tableName;

    BHistoryData();
    BHistoryData(QString tableName);
    ~BHistoryData();

    bool createTable();                    //创建数据表
    bool createTable(QString tableName);   //创建数据表

    bool deleteTable(QString tableName);   //删除指定数据表
    bool initDataTables();                 //初始化监测因子本地数据表（删除所有已经存在的数据表，创建配置的监测因子数据表）
    bool initDataTables(QString portName); //初始化指定端口的所有监测因子数据表

    bool packDataTables();   //压缩数据表。删除 超出 存储时长 的数据。

    QList<QString> getDataTablesList();    //获得 监测因子本地数据表 列表
    QList<QString> getDataTablesList(QString portName);    //获得指定端口的 监测因子本地数据表 列表

    bool writeToDB(MFactor *factor);             //将实时数据写入数据库
    int getTableWritePtr(QString tableName);   //获得文件读写指针

    static bool initCalcTimestamp(); //系统启动时，初始化历史数据计算时间戳 为当前时间

    static bool getTableCalcTimestampM(QString tableName, QString &timestamp);  //获取上传分钟级数据的时间戳（上次上次的最晚分钟级时间）
    static bool getTableCalcTimestampH(QString tableName, QString &timestamp);  //获取上传小时级数据的时间戳（上次上次的最晚小时级时间）
    static bool getTableCalcTimestampD(QString tableName, QString &timestamp);  //获取上传日级数据的时间戳（上次上次的最晚日级时间）

    //获取上传分钟、小时、日级数据的时间戳（上次上次的最晚分钟、小时、日级时间）
    bool getTableCalcTimestamp(QString tableName, QString &timestampM,QString &timestampH,QString &timestampD);

    bool getTablePtr(QString tableName, int &write_ptr, QString &upload_ptr); //获得文件读写指针、上传实时数据指针

    bool putTableWritePtr(QString tableName, int writePtr);   //重置文件读写指针
    bool putTableWritePtr(QString tableName, int writePtr, QString timestamp);   //重置文件读写指针

    bool putTableUploadPtr(QString tableName, QString uploadPtr); //重置上传实时数据的指针

    static bool putTableCalcTimestampM(QString tableName, QString timestamp);  //重置上传分钟级数据的时间戳（上次上次的最晚分钟级时间）
    static bool putTableCalcTimestampH(QString tableName, QString timestamp);  //重置上传小时级数据的时间戳（上次上次的最晚小时级时间）
    static bool putTableCalcTimestampD(QString tableName, QString timestamp);  //重置上传日级数据的时间戳（上次上次的最晚日级时间）
    //重置上传分钟、小时、日级数据的时间戳（上次上次的最晚分钟、小时、日级时间）
    bool putTableCalcTimestamp(QString tableName, QString &timestampM,QString &timestampH,QString &timestampD);


    bool getTimestampById(QString tableName, int id, QDateTime &datetime); //获取指定 ID 的时间戳

    //从数据表中获取指定要素的未上传的实时数据
    bool getSerialData(MFactor * factor, QList<RealTimeData> & data );

    //供绘图用：获取实时数据
    bool getSerialData(MFactor * factor, QVector<double> &values, QVector<double> &timestamps); //从数据表中读取连续的数据（最多读取120条）
    bool getSerialData(QString tableName, QDateTime timeStart, QDateTime timeEnd, QVector<double> &values, QVector<double> &timestamps);
    bool getSerialData(QString tableName, QDateTime timeStart, QDateTime timeEnd, int limit, int offset, QVector<double> &values, QVector<double> &timestamps);

    //获取指定要素指定时间范围内的统计数据：最大、最小、平均
    bool getSerialData(QString factorCode, QDateTime timeStart, QDateTime timeEnd, double max, double min, double avg);

    //供绘图用：获得指定监测因子的指定时间范围内从 offset 开始的最多 limit 条 分钟级数据
    bool getSerialDataM(QString factorCode, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                        QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                        QVector<double> &timestamps);
    //获得指定监测因子的指定时间范围内分钟级数据
    bool getSerialDataM(QString factorCode, QDateTime timeStart, QDateTime timeEnd,
                        QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                        QVector<double> &timestamps);

    //获得指定数据表的指定时间范围内分钟级数据
    bool getSerialDataM(QString factorCode, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data);

    //供绘图用：获得指定监测因子的指定时间范围内从 offset 开始的最多 limit 条 小时级数据
    bool getSerialDataH(QString factorCode, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                        QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                        QVector<double> &timestamps);
    //获得指定数据表的指定时间范围内小时级数据
    bool getSerialDataH(QString factorCode, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data);

    //供绘图用：获得指定数据表的指定时间范围内从 offset 开始的最多 limit 条 日数据
    bool getSerialDataD(QString factorCode, QDateTime timeStart, QDateTime timeEnd, int limit, int offset,
                        QVector<double> &valuesMax, QVector<double> &valuesMin, QVector<double> &valuesAvg,
                        QVector<double> &timestamps);
    //获得指定数据表的指定时间范围内日数据
    bool getSerialDataD(QString factorCode, QDateTime timeStart, QDateTime timeEnd, QList<MStatisticalData> &data);


    //获得指定数据表的指定时间范围内的实时数据记录条数
    int getRecordCount(QString tableName, QDateTime timeStart, QDateTime timeEnd);

    //获得指定数据表的指定时间范围内的分钟级数据记录条数
    int getRecordCountM(QString factorCode, QDateTime timeStart, QDateTime timeEnd);
    //获得指定数据表的指定时间范围内的小时级数据记录条数
    int getRecordCountH(QString factorCode, QDateTime timeStart, QDateTime timeEnd);
    //获得指定数据表的指定时间范围内的日数据记录条数
    int getRecordCountD(QString factorCode, QDateTime timeStart, QDateTime timeEnd);
};



#endif // BHISTORYDATA_H
