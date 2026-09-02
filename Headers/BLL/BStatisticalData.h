#ifndef BSTATISTICALDATA_H
#define BSTATISTICALDATA_H

#include "Headers/Model/MStatisticalData.h"

#include <Headers/DAL/dal.h>

#include "BSysConfig.h"

extern Sqlite bSqlite;
extern QMutex mutexSqlite;

class BStatisticalData
{
private:
   //读取指定因子的指定类型指定时间范围的数据
   bool readData(QString factor, QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围的所有因子的指定类型数据
   bool readData(QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);

   //读取指定因子的指定时间范围未上传的 指定类型的数据，并将上传标识置为 1
   bool outData(QString factor, QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围未上传的 指定类型的所有因子的数据，并将上传标识置为 1
   bool outData(QString dataType, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);

public:
   //将一条统计数据存入数据库
   bool writeToDB(MStatisticalData &data);

   //读取指定因子的指定时间范围分钟级数据
   bool readMinuteData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围所有因子的分钟级数据
   bool readMinuteData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data);

   //读取指定因子的指定时间范围未上传的分钟级数据，并将上传标识置为 1
   bool outMinuteData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围未上传的所有因子分钟级数据，并将上传标识置为 1
   bool outMinuteData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取未上传的所有因子分钟级数据，并将上传标识置为 1
   bool outMinuteData(QList<MStatisticalData> &data);


   //读取指定因子的指定时间范围小时级数据
   bool readHourData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围所有因子的小时级数据
   bool readHourData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data);

   //读取指定因子的指定时间范围未上传的小时级数据，并将上传标识置为 1
   bool outHourData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围未上传的所有因子小时级数据，并将上传标识置为 1
   bool outHourData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取未上传的所有因子小时级数据，并将上传标识置为 1
   bool outHourData(QList<MStatisticalData> &data);

   //读取指定因子的指定时间范围日数据
   bool readDayData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围所有因子的日数据
   bool readDayData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data);

   //读取指定因子的指定时间范围未上传的日数据，并将上传标识置为 1
   bool outDayData(QString factor, QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取指定时间范围未上传的所有因子日数据，并将上传标识置为 1
   bool outDayData(QString timeStart, QString timeEnd, QList<MStatisticalData> &data);
   //读取未上传的所有因子日数据，并将上传标识置为 1
   bool outDayData(QList<MStatisticalData> &data);

   bool packDB(); //清理超时间范围数据

};

#endif // BSTATISTICALDATA_H
