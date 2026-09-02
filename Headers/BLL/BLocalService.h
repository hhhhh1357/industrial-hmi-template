#ifndef BLOCALSERVICE_H
#define BLOCALSERVICE_H

#include "BSysConfig.h"

#include <QString>
#include <QThread>
#include <QTimer>

#include "Sources/DT/Task.h"
#include "Sources/DT/ConnectStatus.h"
#include "Sources/DT/CloudServices.h"

#include <Headers/Model/MFactor.h>
#include <Headers/Model/MStatisticalData.h>

class BLocalService:public QThread
{
    Q_OBJECT

private:
    QTimer *calcStatisticalDataTimer; //计算历史统计数据。计算分钟、小时、日数据

    QMutex mutexUploadRD_Timer;
    bool statusUploadRD_Timer=false;

    QTimer *uploadRD_Timer;  //实时数据上传定时器
    QTimer *uploadMD_Timer;  //分钟数据上传定时器
    QTimer *uploadTimer;     //上传数据定时器
    QTimer *dogTimer; //看门狗定时器
    QTimer *uploadCYQ_Timer;  //采样器数据上传定时器

    QString from;  //对象生成的地方
    QString startRunningTime; //设备开机运行时间

    void writeToUploadList(QList<MStatisticalData> &data);
   // void calcMinuteData();
    void calcHourData();
    void calcDayData();

    bool getHistoryData(QString dataType, QString timeStart, QString timeEnd, QList<QString> &cmdStringList);

signals:
    void requestCloudService(Task task);
public slots:
    void getServerConnectStatus(ConnectStatus Status);
    void getDtu4gStatus( int status );

public:
    BLocalService(QString from, QObject *parent=nullptr);
    BLocalService(QObject *parent=nullptr);
    ~BLocalService();

    BSysConfig sysConfig;

    bool writeToUploadList(QString timestamp, QString cmdStr);

    void startTimers(); //启动所有的定时器

    void startUploadRealData();  //启动实时数据上传 2011
    void stopUploadRealData();   //停止实时数据上传 2012

    void pauseUploadRealData();
    void continueUploadRealData();

    bool setRealDataUploadStatus(bool status); //启动或停止上传实时数据  2011   2012

    void connectToCloudService(CloudServices * cs);  //与 CloudService 建立 信号 和 槽 连接

    //设置数采仪参数。可以供云端下发命令调用
    bool setTimeoutAndRepeat(QString timeout, QString repeat); //设置超时时间及重发次数  1000
    bool setLocalDatetime(QString datetime);  //设置数采仪时间  1012
    bool setLocalRealDataInterval(int interval);//设置数采仪实时数据间隔 1062
    bool setLocalMinuteDataInterval(int interval);//设置数采仪分钟数据间隔 1064
    bool setLocalPassword(QString password); //设置数采仪的密码 1072

    //获取数采仪参数。可以供云端下发命令调用
    QString getLocalDatetime(); //获取数采仪时间  1011

    int getLocalRealDataInterval(); //获取数采仪实时数据间隔 1061
    int getLocalRealDataInterval(QString &cmdString); //获取数采仪实时数据间隔 1061

    int getLocalMinuteDataInterval(); //获取数采仪分钟级数据间隔 1063
    int getLocalMinuteDataInterval(QString cmdString); //获取数采仪分钟级数据间隔 1063

    bool getLocalAllRealData(QMap<QString,QString> & dataList);  //获取所有监测因子实时数据
    bool getLocalRealData(QString factorCode, QMap<QString,QString> & dataList);  //获取指定监测因子实时数据

    bool getLocalAllMinuteData(QString timeStart, QString timeEnd,QMap<QString,QString> & dataList);  //获取所有监测因子分钟数据
    bool getLocalMinuteData(QString factorCode, QString timeStart, QString timeEnd,QMap<QString,QString> & dataList);  //获取指定监测因子分钟数据

    bool getLocalAllHourData(QString timeStart, QString timeEnd,QMap<QString,QString> & dataList);  //获取所有监测因子小时数据
    bool getLocalHourData(QString factorCode, QString timeStart, QString timeEnd,QMap<QString,QString> & dataList);  //获取指定监测因子小时数据

    //供云端取数据调用
    bool getLocalMinuteData(QString timeStart, QString timeEnd, QList<QString> &cmdStringList); //读取所有因子的分钟数据，生成上传命令
    bool getLocalHourData(QString timeStart, QString timeEnd, QList<QString> &cmdStringList); //读取所有因子的小时数据，生成上传命令
    bool getLocalDayData(QString timeStart, QString timeEnd, QList<QString> &cmdStringList); //读取所有因子的日数据，生成上传命令

    QString getStartRunningTime();  //获取数采仪开机时间上传命令 2018


    bool findFactor(QString factorCode, MFactor &factor); //查询监测因子的详细配置数据
    QList<MFactor> getAllFactor(); //获取所有监测因子列表

    //向发云端请求
    bool requestSynchronizeDatetime(); //请求与云端同步数采仪时间 1013

    bool requestUploadRealDataInterval();//请求向云端上传数采仪实时数据间隔 1061
    bool requestUploadMinuteDataInterval();//请求向云端上传数采仪分钟数据间隔 1061

    bool requestUploadRealData();//请求向云端上传数采仪实时数据    2011
    bool requestUploadMimuteData();//请求向云端上传数采仪分钟数据    2051
    bool requestUploadHourData();//请求向云端上传数采仪小时数据   2061
    bool requestUploadDayData();//请求向云端上传数采仪日数据   2031
    bool requestStartRunningTime();  //上传数采仪开机时间 2018
    bool requestUploadCYQ();  //请求向云端上传数采仪采样器数据    3020

    //通过定时器向云端传数据
    bool requestUploadData(); //请求向云端重新上传数采仪数据
    bool requestUploadData(QString cmdString);
};

class CLog
{
private:
    CLog();
    CLog(const CLog &);
    CLog & operator = (const CLog &);

public:
    static CLog * getInstance();
    void log(QString cmdString);
};

#endif // BLOCALSERVICE_H
