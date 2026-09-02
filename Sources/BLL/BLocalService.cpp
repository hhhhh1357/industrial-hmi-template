#include "Headers/BLL/BLocalService.h"

#include <Headers/BLL/BComArgs.h>
#include <Headers/BLL/BFactor.h>
#include <Headers/BLL/BHistoryData.h>
#include <Headers/BLL/BStatisticalData.h>
#include <Headers/BLL/BSysDictionary.h>
#include <Headers/BLL/BUser.h>

#include <QDateTime>
#include <QException>
#include <mainwindow.h>

#include <Sources/Tools/datatype.h>
#include <Sources/Tools/sysutil.h>

#include "Sources/Tools/feeddog.h"
#include <Sources/DT/SamplerState.h>

extern MainWindow * mainWindow;
extern Sqlite bSqlite;
extern SamplerState bSamplerState;

BLocalService::BLocalService(QObject *parent)
{

}

BLocalService::~BLocalService()
{
    if (this->from=="LOCAL")
    {
        calcStatisticalDataTimer->stop();
        delete calcStatisticalDataTimer;

        uploadRD_Timer->stop();
        delete  uploadRD_Timer;

        uploadMD_Timer->stop();
        delete uploadMD_Timer;

        uploadTimer->stop();
        delete uploadTimer;

        dogTimer->stop();
        delete  dogTimer;

        uploadCYQ_Timer->stop();
        delete  uploadCYQ_Timer;
    }
}

void BLocalService::startTimers()
{
    if (calcStatisticalDataTimer->isActive()==false)
    {
        calcStatisticalDataTimer->start();
    }

    if (uploadRD_Timer->isActive() == false)
    {
        if (sysConfig.SysDic.getDATA_UPLOAD_REAL_ENABLED())
        {
            int inv = sysConfig.SysDic.getDATA_UPLOAD_REAL();
            uploadRD_Timer->setInterval(inv*1000);
            uploadRD_Timer->start();
            statusUploadRD_Timer=true;
        }
    }

    if (uploadMD_Timer->isActive() == false)        
    {
       uploadMD_Timer->start();
    }
    if (uploadTimer->isActive() == false)
    {
       uploadTimer->start();
    }

    if (dogTimer->isActive()==false)
    {
        dogTimer->start();
    }

    if (uploadCYQ_Timer->isActive()==false)
    {
        uploadCYQ_Timer->start();
    }
}

//启动实时数据上传
void BLocalService::startUploadRealData()
{
    QMutexLocker locker(&(this->mutexUploadRD_Timer));
    this->statusUploadRD_Timer = true;
    sysConfig.SysDic.putDATA_UPLOAD_REAL_ENABLED(true);
}
//停止实时数据上传
void BLocalService::stopUploadRealData()
{
    QMutexLocker locker(&(this->mutexUploadRD_Timer));
    this->statusUploadRD_Timer = false;
    sysConfig.SysDic.putDATA_UPLOAD_REAL_ENABLED(false);
}

void BLocalService::pauseUploadRealData()
{
    QMutexLocker locker(&(this->mutexUploadRD_Timer));
    this->statusUploadRD_Timer = false;
}

void BLocalService::continueUploadRealData()
{
    QMutexLocker locker(&(this->mutexUploadRD_Timer));
    this->statusUploadRD_Timer = true;
}

bool BLocalService::setRealDataUploadStatus(bool status) //启动或停止上传实时数据  2011   2012
{
    {
        QMutexLocker locker(&(this->mutexUploadRD_Timer));
        this->statusUploadRD_Timer = status;
    }
    sysConfig.SysDic.putDATA_UPLOAD_REAL_ENABLED(status);
}

void BLocalService::connectToCloudService(CloudServices * cs)
{
    connect(this, &BLocalService::requestCloudService, cs, &CloudServices::handRequest);
    connect(cs,&CloudServices::reportConnectionState,this,&BLocalService::getServerConnectStatus);
}

//将命令串写入到上传列表数据表中
bool BLocalService::writeToUploadList(QString timestamp, QString cmdStr)
{
    QMap<QString,QString> data;
    data.insert("timestamp","'"+timestamp+"'");
    data.insert("uploadCmd","'"+cmdStr+"'");
    return bSqlite.add("TUploadDataList",data);
}

//将需要上传的数据放入到上传列表中
void BLocalService::writeToUploadList(QList<MStatisticalData> &data)
{
    int i=0;
    int maxRec=60;
    QString cmdString;
    QString dt;
    QString timestamp="";

    MFactor factor;
    bool w00000=false;

    w00000=findFactor("w00000",factor);

qDebug()<<"&&&&&&&&&&&data.count()= "<<data.count()<<endl;

    for (int i = 0;i < data.count(); i++)
    {
        if (data[i].timestamp!=timestamp)
        {
            //i=0;
            timestamp=data[i].timestamp;
        }

        if (i==0)
        {
            QString QN = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
            if (data.at(0).dataType=="M") //分钟数据
            {
               dt = QDateTime::fromString(data[i].timestamp,"yyyy-MM-dd HH:mm").toString("yyyyMMddHHmmss");
               cmdString = "QN=%1;ST=%2;CN=2051;PW=123456;MN=%3;Flag=5;CP=&&";
            }
            else if (data.at(0).dataType=="H")  //小时数据
            {
               dt = QDateTime::fromString(data[i].timestamp,"yyyy-MM-dd HH").toString("yyyyMMddHHmmss");
               cmdString = "QN=%1;ST=%2;CN=2061;PW=123456;MN=%3;Flag=5;CP=&&";
            }
            else if (data.at(0).dataType=="D")  //日数据
            {
               dt = QDateTime::fromString(data[i].timestamp,"yyyy-MM-dd").toString("yyyyMMddHHmmss");
               cmdString = "QN=%1;ST=%2;CN=2031;PW=123456;MN=%3;Flag=5;CP=&&";
            }
            cmdString = cmdString.arg(QN).arg(sysConfig.SysDic.getST()).arg(sysConfig.SysDic.getMN());

            cmdString += "DataTime=%1;";
            cmdString = cmdString.arg(dt);
        }

        MFactor factor;
        BFactor bf;
        bf.findFactor(data[i].code,factor);
        int in,dn;
        FactorDataType::getDataLength(factor.nType,in,dn);

        if (data[i].code.left(1)=="w" && w00000) //污水，有累计流量. 统计累计流量
        {
            cmdString += "%1-Cou=%2;";
            if (dn==0)
               cmdString = cmdString.arg(data[i].code).arg(QString::number((int)(data[i].valueCou+0.5)));
            else
               cmdString = cmdString.arg(data[i].code).arg(QString::number(data[i].valueCou,'f',dn));
        }
        if (dn==0)
        {
            cmdString += "%1-Max=%2;";
            cmdString = cmdString.arg(data[i].code).arg(QString::number((int)(data[i].valueMax+0.5)));
            cmdString += "%1-Min=%2;";
            cmdString = cmdString.arg(data[i].code).arg(QString::number((int)(data[i].valueMin+0.5)));
            cmdString += "%1-Avg=%2;";
            cmdString = cmdString.arg(data[i].code).arg(QString::number((int)(data[i].valueAvg+0.5)));
            cmdString += "%1-Flag=%2;";
            cmdString = cmdString.arg(data[i].code).arg(data[i].dataFlag);
        }
        else
        {
            cmdString += "%1-Max=%2;";
            cmdString = cmdString.arg(data[i].code).arg(QString::number(data[i].valueMax,'f',dn));
            cmdString += "%1-Min=%2;";
            cmdString = cmdString.arg(data[i].code).arg(QString::number(data[i].valueMin,'f',dn));
            cmdString += "%1-Avg=%2;";
            cmdString = cmdString.arg(data[i].code).arg(QString::number(data[i].valueAvg,'f',dn));
            cmdString += "%1-Flag=%2;";
            cmdString = cmdString.arg(data[i].code).arg(data[i].dataFlag);
        }
        if (i>=maxRec)
        {
            cmdString +="&&";
            requestUploadData(cmdString);

            //writeToUploadList(dt,cmdString);
            i=0;
        }
    }
    if (i<maxRec)
    {
        cmdString +="&&";
        requestUploadData(cmdString);
        //writeToUploadList(dt,cmdString);
    }
    //qDebug()<<"&&&&&&&&&&&cmdString= "<<cmdString<<endl;
}

//小时数据统计
void BLocalService::calcHourData()
{
    QDateTime datetimeNow = QDateTime::currentDateTime();
    QList<MFactor*> dataList = mainWindow->getDataItemList();
    MFactor *factor;
    BHistoryData hisData;
    BStatisticalData staData;
    QList <MStatisticalData> data,dataH;
    for (int i=0;i<dataList.count();i++)
    {
        factor=dataList.at(i);
        QString tt;
        QDateTime timeStart,timeEnd;
        if (hisData.getTableCalcTimestampH(factor->localTableName,tt))
        {
            timeStart = QDateTime::fromString(tt,"yyyy-MM-dd HH");
            timeEnd = timeStart.addSecs(60*60);
            if (timeEnd<=datetimeNow)
            {
                while (timeEnd<=datetimeNow)
                {
                     //获得1小时内的分钟数据
                     hisData.getSerialDataM(factor->code,timeStart, timeEnd, data);
                     if (data.length()>0) //查到的数据不为空
                     {
                         MStatisticalData d;
                         d.code = factor->code;
                         d.dataType = "H";
                         d.valueAvg = 0;
                         d.valueMax = 0;
                         d.valueMin = 0;
                         d.valueCou = 0;
                         d.timestamp = timeStart.toString("yyyy-MM-dd HH");
                         d.dataFlag="N";
                         foreach( MStatisticalData dd, data )
                         {
                             d.valueAvg += dd.valueAvg;
                             d.valueMax += dd.valueMax;
                             d.valueMin += dd.valueMin;
                             d.valueCou += dd.valueCou;
                             if (dd.dataFlag != "N")   //只要有一个分钟数据异常，就标识小时数据异常
                                 d.dataFlag = dd.dataFlag;
                         }

                         d.valueAvg /= data.length();
                         d.valueMax /= data.length();
                         d.valueMin /= data.length();
                         d.valueCou /= data.length();

                         dataH.append(d);
                         data.clear();
                     }
                     timeStart = timeEnd;
                     timeEnd = timeEnd.addSecs(60*60);
                }

                foreach( MStatisticalData dd, dataH )
                {
                    staData.writeToDB(dd);
                }

                hisData.putTableCalcTimestampH(factor->localTableName,timeStart.toString("yyyy-MM-dd HH"));

                dataH.clear();
                usleep(50);
            }
        }
    }
}
//日数据统计
void BLocalService::calcDayData()
{
    QDateTime datetimeNow = QDateTime::currentDateTime();
    QList<MFactor*> dataList = mainWindow->getDataItemList();
    MFactor *factor;
    BHistoryData hisData;
    BStatisticalData staData;
    QList <MStatisticalData> data,dataD;
    for (int i=0;i<dataList.count();i++)
    {
        factor=dataList.at(i);
        QString tt;
        QDateTime timeStart,timeEnd;
        if (hisData.getTableCalcTimestampD(factor->localTableName,tt))
        {
            timeStart = QDateTime::fromString(tt,"yyyy-MM-dd");
            timeEnd = timeStart.addSecs(24*60*60);
            if (timeEnd<=datetimeNow)
            {
                while (timeEnd<=datetimeNow)
                {
                     //查当天的小时数据
                     hisData.getSerialDataH(factor->code,timeStart, timeEnd, data);

                     if (data.length()>0) //查到的数据不为空
                     {
                         MStatisticalData d;
                         d.code = factor->code;
                         d.dataType = "D";
                         d.valueAvg = 0;
                         d.valueMax = 0;
                         d.valueMin = 0;
                         d.valueCou = 0;
                         d.timestamp = timeStart.toString("yyyy-MM-dd");
                         d.dataFlag="N";
                         foreach( MStatisticalData dd, data )
                         {
                             d.valueAvg += dd.valueAvg;
                             d.valueMax += dd.valueMax;
                             d.valueMin += dd.valueMin;
                             d.valueCou += dd.valueCou;
                             if (dd.dataFlag != "N")   //只要有一个分钟数据异常，就标识小时数据异常
                                 d.dataFlag = dd.dataFlag;
                         }
                         d.valueAvg /= data.length();
                         d.valueMax /= data.length();
                         d.valueMin /= data.length();
                         d.valueCou /= data.length();
                         dataD.append(d);
                         data.clear();
                     }

                     timeStart = timeEnd;
                     timeEnd = timeEnd.addSecs(24*60*60);
                }
                hisData.putTableCalcTimestampD(factor->localTableName,timeStart.toString("yyyy-MM-dd"));
                foreach( MStatisticalData dd, dataD )
                {
                    staData.writeToDB(dd);
                }
                dataD.clear();
                usleep(50);
            }
        }
    }
}

bool BLocalService::getHistoryData(QString dataType, QString timeStart, QString timeEnd, QList<QString> &cmdStringList)
{
   BStatisticalData staData;
   QList<MStatisticalData> data;
   bool r;
   if (dataType=="M")
       r=staData.readMinuteData(timeStart,timeEnd,data);
   else if (dataType=="H")
       r=staData.readHourData(timeStart,timeEnd,data);
   else if (dataType=="D")
       r = staData.readDayData(timeStart, timeEnd, data);
   else
       return false;

   if (data.count()==0)
       return false;

   int i=0;
   int maxRec=60;
   QString cmdString="";
   QString dt;
   QString timestamp="";

   foreach (MStatisticalData msd, data)
   {
       if (msd.timestamp!=timestamp)
       {
           if (cmdString.length()>0)
           {
               cmdString +="&&";
               cmdStringList.append(cmdString);
           }
           i=0;
           timestamp=msd.timestamp;
       }

       if (i==0)
       {
           msleep(1);
           QString QN = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
           if (data.at(0).dataType=="M") //分钟数据
           {
              dt = QDateTime::fromString(msd.timestamp,"yyyy-MM-dd HH:mm").toString("yyyyMMddHHmmss");
              cmdString = "QN=%1;ST=%2;CN=2051;PW=123456;MN=%3;Flag=4;CP=&&";
           }
           else if (data.at(0).dataType=="H")  //小时数据
           {
              dt = QDateTime::fromString(msd.timestamp,"yyyy-MM-dd HH").toString("yyyyMMddHHmmss");
              cmdString = "QN=%1;ST=%2;CN=2061;PW=123456;MN=%3;Flag=4;CP=&&";
           }
           else if (data.at(0).dataType=="D")  //日数据
           {
              dt = QDateTime::fromString(msd.timestamp,"yyyy-MM-dd").toString("yyyyMMddHHmmss");
              cmdString = "QN=%1;ST=%2;CN=2031;PW=123456;MN=%3;Flag=4;CP=&&";
           }
           cmdString = cmdString.arg(QN).arg(sysConfig.SysDic.getST()).arg(sysConfig.SysDic.getMN());

           cmdString += "DataTime=%1;";
           cmdString = cmdString.arg(dt);
       }

       MFactor factor;
       BFactor bf;
       bf.findFactor(msd.code,factor);
       int in,dn;
       FactorDataType::getDataLength(factor.nType,in,dn);

       if (msd.code.left(1)=="w") //污水，有累计流量
       {
           cmdString += "%1-Cou=%2;";
           if (dn==0)
              cmdString = cmdString.arg(msd.code).arg(QString::number((int)(msd.valueCou+0.5)));
           else
              cmdString = cmdString.arg(msd.code).arg(QString::number(msd.valueCou,'f',dn));
       }
       if (dn==0)
       {
           cmdString += "%1-Max=%2;";
           cmdString = cmdString.arg(msd.code).arg(QString::number((int)(msd.valueMax+0.5)));
           cmdString += "%1-Min=%2;";
           cmdString = cmdString.arg(msd.code).arg(QString::number((int)(msd.valueMin+0.5)));
           cmdString += "%1-Avg=%2;";
           cmdString = cmdString.arg(msd.code).arg(QString::number((int)(msd.valueAvg+0.5)));
           cmdString += "%1-Flag=%2;";
           cmdString = cmdString.arg(msd.code).arg(msd.dataFlag);
       }
       else
       {
           cmdString += "%1-Max=%2;";
           cmdString = cmdString.arg(msd.code).arg(QString::number(msd.valueMax,'f',dn));
           cmdString += "%1-Min=%2;";
           cmdString = cmdString.arg(msd.code).arg(QString::number(msd.valueMin,'f',dn));
           cmdString += "%1-Avg=%2;";
           cmdString = cmdString.arg(msd.code).arg(QString::number(msd.valueAvg,'f',dn));
           cmdString += "%1-Flag=%2;";
           cmdString = cmdString.arg(msd.code).arg(msd.dataFlag);
       }
       i++;
       if (i>=maxRec)
       {
           cmdString +="&&";
           cmdStringList.append(cmdString);
           i=0;
       }
   }
   if (i<maxRec)
   {
       cmdString +="&&";
       cmdStringList.append(cmdString);
   }

   if (cmdStringList.count()>1) //如果有多包
   {
       int PNUM=cmdStringList.count();
       for (int PNO=1;PNO<=cmdStringList.count();PNO++)
       {           
           cmdStringList.operator[](PNO-1).replace("Flag=4","Flag=6");
           QString P=QString("PNUM=%1;PNO=%2;CP").arg(PNUM).arg(PNO);
           cmdStringList.operator[](PNO-1).replace("CP",P);
       }
   }

   return r;
}

void BLocalService::getServerConnectStatus(ConnectStatus Status)
{
    for (int i=0;i<sysConfig.ServerList.count();i++)
    {
        if (Status.ip == sysConfig.ServerList.at(i)->ip)
        {
            if (Status.network_state)
               sysConfig.ServerList.at(i)->status=1;
            else
               sysConfig.ServerList.at(i)->status=0;
            break;
        }
    }
}

void BLocalService::getDtu4gStatus(int status )
{
    sysConfig.dtu4G.status=status;
}

BLocalService::BLocalService(QString from, QObject *parent)
{
    this->from = from;
    if (from=="LOCAL")
    {
        this->startRunningTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");

        //计算历史统计数据定时器
        this->calcStatisticalDataTimer = new QTimer();
        connect(this->calcStatisticalDataTimer,&QTimer::timeout,[=](){
 //qDebug()<<"Start calcStatisticalDataTimer......"<<endl;
            static bool isFinishedDay=false;
            static bool isFinishedHour=false;
            static bool isFinishedMinute=false;
            static bool isFinishedPack=false;

            this->calcStatisticalDataTimer->stop();
            //计算历史统计数据
            QDateTime dt = QDateTime::currentDateTime();
            if (isFinishedHour==false && dt.time().minute()<=1) //xx:00 或 xx:01 统计小时数据
            {
                //qDebug()<<"Start calcHourData......"<<endl;
                //统计小时数据
                calcHourData();
                isFinishedHour = true;
            }
            else if (isFinishedPack==false && (dt.time().minute()==04||dt.time().minute()==05)) //xx:04 或 xx:05
            {
                //qDebug()<<"Start packDataTables......"<<endl;
                //删除超过存储时间的
                BHistoryData his;
                his.packDataTables();
                isFinishedPack = true;
            }
            else if (isFinishedDay==false &&(dt.time().hour()==0 && (dt.time().minute()==2||dt.time().minute()==3))) //00:02 或 00:03 统计日数据
            {
                //qDebug()<<"Start calcDayData......"<<endl;
                //统计日数据
                calcDayData();
                isFinishedDay = true;
            }
            else if (dt.time().minute()==06||dt.time().minute()==07)
            {
                isFinishedDay=false;
                isFinishedHour=false;
                isFinishedMinute=false;
                isFinishedPack=false;
            }
            this->calcStatisticalDataTimer->start();
            });

        this->calcStatisticalDataTimer->setInterval(30*1000);  //触发时间间隔为 30秒 触发一次

        this->uploadRD_Timer = new QTimer();
        connect(this->uploadRD_Timer,&QTimer::timeout,[=](){
 //qDebug()<<"Start uploadRD_Timer......"<<endl;
            this->uploadRD_Timer->stop();
            this->requestUploadRealData();  //上传实时数据
            this->uploadRD_Timer->start();
            });
        int inv = sysConfig.SysDic.getDATA_UPLOAD_REAL();
        this->uploadRD_Timer->setInterval(inv*1000);
//上传水质采样器数据
        this->uploadCYQ_Timer = new QTimer();
        connect(this->uploadCYQ_Timer,&QTimer::timeout,[=](){
            this->uploadCYQ_Timer->stop();
            this->requestUploadCYQ();  //上传采样器数据
            this->uploadCYQ_Timer->start();
            });
        this->uploadCYQ_Timer->setInterval(inv*1000);
////////////////////

        this->uploadMD_Timer = new QTimer();
        connect(this->uploadMD_Timer,&QTimer::timeout,[=](){
            QDateTime dt = QDateTime::currentDateTime();
            QDateTime dt_m = QDateTime::fromString( dt.toString("yyyy-MM-dd HH:mm"),"yyyy-MM-dd HH:mm");
            QDateTime dt_h = QDateTime::fromString( dt.toString("yyyy-MM-dd HH"),"yyyy-MM-dd HH");
            QDateTime dt_d = QDateTime::fromString( dt.addDays(-1).toString("yyyy-MM-dd"),"yyyy-MM-dd");

            static QDateTime lasttimeM= dt_m,lasttimeH=dt_h,lasttimeD=dt_d;

//qDebug()<<"Start lasttimeM :"<<lasttimeM.toString("yyyy-MM-dd HH:mm:ss")<<endl;
//qDebug()<<"Start lasttimeH :"<<lasttimeH.toString("yyyy-MM-dd HH:mm:ss")<<endl;
//qDebug()<<"Start lasttimeD :"<<lasttimeD.toString("yyyy-MM-dd HH:mm:ss")<<endl;
            this->uploadMD_Timer->stop();

            //上传分钟数据
            if (lasttimeM.addSecs(sysConfig.SysDic.getDATA_UPLOAD_MINUTE()/60)<=dt)
            {
                //qDebug()<<"Start upload minute's Data......"<<dt.toString("yyyy-MM-dd HH:mm:ss")<<endl;
                if (this->requestUploadMimuteData())  //上传分钟数据成功，修改上传分钟数据时间戳
                    lasttimeM = dt;
            }

//  qDebug()<<"dt = "<<dt.toString("yyyy-MM-dd HH:mm:ss")<<endl;
//  qDebug()<<"lasttimeH = "<<lasttimeH.toString("yyyy-MM-dd HH:mm:ss")<<endl;
//  qDebug()<<"lasttimeH+1 = "<<lasttimeH.addSecs(sysConfig.SysDic.getDATA_UPLOAD_HOUR()).toString("yyyy-MM-dd HH:mm:ss")<<endl;
            //上传小时数据: 整点上传
            if ( dt.time().minute()==0 && lasttimeH.addSecs(sysConfig.SysDic.getDATA_UPLOAD_HOUR())<=dt)
            {
                //qDebug()<<"Start upload hour's Data......"<<dt.toString("yyyy-MM-dd HH:mm:ss")<<endl;
                if (this->requestUploadHourData()) //上传小时数据
                    lasttimeH = QDateTime::fromString( dt.toString("yyyy-MM-dd HH"),"yyyy-MM-dd HH");;
            }
            //上传日数据
            if ( dt.time().hour()==sysConfig.SysDic.getDATA_UPLOAD_DAY() && lasttimeD.date().day() != dt.date().day() )
            {
                //qDebug()<<"Start upload day's Data......"<<dt.toString("yyyy-MM-dd HH:mm:ss")<<endl;
                if (this->requestUploadDayData()) //上传日数据
                    lasttimeD = QDateTime::fromString( dt.toString("yyyy-MM-dd"),"yyyy-MM-dd");
            }

            this->uploadMD_Timer->start();
            });
        this->uploadMD_Timer->setInterval(1000);

        //上传数据的定时器
        this->uploadTimer = new QTimer();
        connect(this->uploadTimer,&QTimer::timeout,[=](){
 //qDebug()<<"Start uploadTimer......"<<endl;
            this->uploadTimer->stop();
            this->requestUploadData();
            this->uploadTimer->start();

            QMutexLocker locker(&(this->mutexUploadRD_Timer));
            if (this->statusUploadRD_Timer && this->uploadRD_Timer->isActive()==false)
                this->uploadRD_Timer->start();
            else if (this->statusUploadRD_Timer==false && this->uploadRD_Timer->isActive()==true)
                this->uploadRD_Timer->stop();

            });
        this->uploadTimer->setInterval(10000);  //每10秒启动一次上传

        //看门狗定时器
        this->dogTimer = new QTimer();
        connect(this->dogTimer,&QTimer::timeout,[=](){

            //看门狗巡逻
            feeddog();

            //qDebug()<<" feeddog!!! ------"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss")<<endl;


            });
        inv = sysConfig.SysDic.getDOG_TIMER();
        this->dogTimer->setInterval(inv*1000);  //启动一次上传，

    }
}

//1000 设置超时时间和重发次数
bool BLocalService::setTimeoutAndRepeat(QString timeout, QString repeat)
{
    try {
        int t = timeout.toInt();
        int r = repeat.toInt();
        sysConfig.SysDic.putTIMEOUT(t);
        sysConfig.SysDic.putREPEAT(r);

        return true;
    } catch (QException e) {
        return false;
    }
}

bool BLocalService::setLocalDatetime(QString datetime)
{
    QDateTime dt = QDateTime::fromString(datetime,"yyyyMMddHHmmss");
    SysUtil::setDatetime( dt );
}

bool BLocalService::setLocalRealDataInterval(int interval)
{
    return sysConfig.SysDic.putINTERVAL(interval); //设置实时数据采样时间间隔
}

bool BLocalService::setLocalMinuteDataInterval(int interval)
{
    return sysConfig.SysDic.putDATA_UPLOAD_MINUTE(interval*60); //设置分钟数据上传时间间隔
}

bool BLocalService::setLocalPassword(QString password) //设置数采仪访问密码
{
    BUser bu;
    bu.setPassword("admin",password);
}

//1011 获取本地时间
QString BLocalService::getLocalDatetime()
{
    return QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
}

int BLocalService::getLocalRealDataInterval()
{
    int interval=sysConfig.SysDic.getINTERVAL(); //实时数据采样间隔
    return interval;
}

int BLocalService::getLocalRealDataInterval(QString &cmdString)
{
    int interval=sysConfig.SysDic.getINTERVAL(); //实时数据采样间隔
    cmdString = "QN=%1;ST=%2;CN=1061;PW=123456;MN=%3;Flag=4;CP=&&RtdInterval=%4&&";
    QString dt = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    cmdString = cmdString.arg(dt);
    cmdString = cmdString.arg(sysConfig.SysDic.getST());
    cmdString = cmdString.arg(sysConfig.SysDic.getMN());
    cmdString = cmdString.arg(interval);
    return interval;
}

int BLocalService::getLocalMinuteDataInterval()
{
    int interval=sysConfig.SysDic.getDATA_UPLOAD_MINUTE()/60; //分钟数据上传间隔
    return interval;
}

int BLocalService::getLocalMinuteDataInterval(QString cmdString)
{
    int interval=sysConfig.SysDic.getDATA_UPLOAD_MINUTE()/60; //分钟数据上传间隔
    cmdString = "QN=%1;ST=%2;CN=1063;PW=123456;MN=%3;Flag=4;CP=&&MinInterval=%4&&";
    QString dt = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    cmdString = cmdString.arg(dt);
    cmdString = cmdString.arg(sysConfig.SysDic.getST());
    cmdString = cmdString.arg(sysConfig.SysDic.getMN());
    cmdString = cmdString.arg(interval);
    return interval;
}

bool BLocalService::getLocalMinuteData(QString timeStart, QString timeEnd, QList<QString> &cmdStringList)
{
    timeStart = QDateTime::fromString(timeStart,"yyyyMMddHHmmss").toString("yyyy-MM-dd HH:mm");
    timeEnd = QDateTime::fromString(timeEnd,"yyyyMMddHHmmss").toString("yyyy-MM-dd HH:mm");
    return getHistoryData("M",timeStart,timeEnd,cmdStringList);
}

bool BLocalService::getLocalHourData(QString timeStart, QString timeEnd, QList<QString> &cmdStringList)
{
    timeStart = QDateTime::fromString(timeStart,"yyyyMMddHHmmss").toString("yyyy-MM-dd HH");
    timeEnd = QDateTime::fromString(timeEnd,"yyyyMMddHHmmss").toString("yyyy-MM-dd HH");
    return getHistoryData("H",timeStart,timeEnd,cmdStringList);
}

bool BLocalService::getLocalDayData(QString timeStart, QString timeEnd, QList<QString> &cmdStringList)
{
    timeStart = QDateTime::fromString(timeStart,"yyyyMMddHHmmss").toString("yyyy-MM-dd");
    timeEnd = QDateTime::fromString(timeEnd,"yyyyMMddHHmmss").toString("yyyy-MM-dd");
    return getHistoryData("D",timeStart,timeEnd,cmdStringList);
}

//2081 获得数采仪开机运行时间 上传命令串
QString BLocalService::getStartRunningTime()
{
    QString cmdString ="QN=%1;ST=%2;CN=2081;PW=123456;MN=%3;Flag=5;CP=&&DataTime=%4;RestartTime=%5&&";
    QString dt = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    cmdString = cmdString.arg(dt);
    cmdString = cmdString.arg(sysConfig.SysDic.getST());
    cmdString = cmdString.arg(sysConfig.SysDic.getMN());
    cmdString = cmdString.arg(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"));
    cmdString = cmdString.arg(this->startRunningTime);
    return cmdString;
}

bool BLocalService::findFactor(QString factorCode, MFactor &factor)
{
    QMutexLocker locker1(&(BComArgs::mutexComArgsList));

    for (int i=0;i<BComArgs::ComArgsList.count();i++)
    {
        for (int j=0;j<BComArgs::ComArgsList.at(i)->ComFactorArgsList.count();j++)
        {
            if ((BComArgs::ComArgsList.at(i))->ComFactorArgsList.at(j).code == factorCode)
            {
                factor = ((BComArgs::ComArgsList.at(i))->ComFactorArgsList.at(j));
                return true;
            }
        }
    }
    return false;
}

//获取所有监测因子信息
QList<MFactor> BLocalService::getAllFactor()
{
    QList <MFactor> listFactors;
    QMutexLocker locker1(&(BComArgs::mutexComArgsList));

    for (int i=0;i<BComArgs::ComArgsList.count();i++)
    {
        for (int j=0;j<BComArgs::ComArgsList.at(i)->ComFactorArgsList.count();j++)
        {
            if ((BComArgs::ComArgsList.at(i))->ComFactorArgsList.at(j).flag == 0)
            {
                MFactor factor = ((BComArgs::ComArgsList.at(i))->ComFactorArgsList.at(j));
//                if (factor.code=="w00000")
//                    continue;
                listFactors.append(factor);
            }
        }
    }
    return listFactors;
}


bool BLocalService::requestSynchronizeDatetime()
{
    QString dt = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    QString cmdString = "QN=%1;ST=%2;CN=1013;PW=123456;MN=%3;Flag=5;CP=&&&&";
    cmdString = cmdString.arg(dt);
    cmdString = cmdString.arg(sysConfig.SysDic.getST());
    cmdString = cmdString.arg(sysConfig.SysDic.getMN());

    return requestUploadData(cmdString);

    //return writeToUploadList(dt,cmdString);
}

//请求上传采样器数据
bool BLocalService::requestUploadCYQ()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString dtt;
    QString cmdString;

    cmdString = "QN=%1;ST=%2;CN=3020;PW=123456;MN=%3;Flag=4;CP=&&DataTime=%4;";
    dtt = dt.toString("yyyyMMddHHmmsszzz");
    cmdString = cmdString.arg(dtt);
    cmdString = cmdString.arg(sysConfig.SysDic.getST());
    cmdString = cmdString.arg(sysConfig.SysDic.getMN());
    cmdString = cmdString.arg((dt.toString("yyyyMMddHHmmss")));

    cmdString += bSamplerState.UpCYQ;
    cmdString += "&&";
    if(bSamplerState.UpCYQ != "")
    {
        requestUploadData(cmdString);
    }
}

//请求上传实时数据
bool BLocalService::requestUploadRealData()
{
    BHistoryData historyData;
    QList<RealTimeData> data;
    QDateTime dt = QDateTime::currentDateTime();
    QString uploadPtr = dt.toString("yyyy-MM-dd HH:mm:ss:zzz");

    QList<MFactor*> dataList = mainWindow->getDataItemList();
    MFactor *factor;
    printf("dataList.count() = %d \n",dataList.count());
    for (int i=0;i<dataList.count();i++)
    {
        factor=dataList.at(i);         
        bool r = historyData.getSerialData(factor,data);  //获取数据。数据包括：从上次未上传数据开始
        if (r==false) continue;
        historyData.putTableUploadPtr(factor->localTableName,uploadPtr); //将数据上传指针修改为当前时间。
        msleep(10);
    }
    printf("data.count() = %d \n",data.count());
    if (data.count()==0)
        return false;

    qSort(data.begin(),data.end(),[](const RealTimeData & a, const RealTimeData & b){ return a.timestamp<b.timestamp;});

    printf("data.count() = %d \n",data.count());
    //用于控制每个数据的数据条数
    int countRM = 5;
    int countR = 0;

    QString dtt;
    QString cts="";
    QString cmdString;
    for(int i=0;i<data.count();i++)
    {
        if (cts!=data.at(i).timestamp || countR==0)
        {
            if (cts!="")
            {
                cmdString+="&&";
                requestUploadData(cmdString);
                msleep(10);
            }

            cmdString = "QN=%1;ST=%2;CN=2011;PW=123456;MN=%3;Flag=5;CP=&&DataTime=%4;";
            dtt = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
            cmdString = cmdString.arg(dtt);
            cmdString = cmdString.arg(sysConfig.SysDic.getST());
            cmdString = cmdString.arg(sysConfig.SysDic.getMN());
            cmdString = cmdString.arg((QDateTime::fromString(data.at(i).timestamp,"yyyy-MM-dd HH:mm:ss").toString("yyyyMMddHHmmss")));
        }
        cmdString += "%5-Rtd=%6;%5-Flag=%7;";
        cmdString = cmdString.arg(data.at(i).code);
        int in,dn;
        FactorDataType::getDataLength(factor->nType,in,dn);
        if (dn==0)
        {
           cmdString = cmdString.arg((int)(data.at(i).value+0.5));
        }
        else
        {
            cmdString = cmdString.arg(QString::number(data.at(i).value,'f',4));//dn
        }
        cmdString = cmdString.arg(data.at(i).dataFlag);
        cts=data.at(i).timestamp;
        countR++;
        if (i>=countRM)
            countR = 0;
        msleep(10);//上传因子间的时间间隔
    }
//数据补传
    //if(data.count() != dataList.count())
    {
        qDebug()<<"buchuan= "<<endl;
        for (int i=0;i<dataList.count();i++)
        {
            qDebug()<<"dataList.at(i)->code= "<<dataList.at(i)->code<<endl;
            qDebug()<<"dataList.at(i)->value= "<<dataList.at(i)->value<<endl;
            qDebug()<<"cmdString.indexOf(dataList.at(i)->code)= "<<cmdString.indexOf(dataList.at(i)->code)<<endl;
            if(cmdString.indexOf(dataList.at(i)->code) < 0)
            {
                qDebug()<<"EntercmdString= "<<cmdString<<endl;
                cmdString += "%8-Rtd=%9;%8-Flag=%10;";
                cmdString = cmdString.arg(dataList.at(i)->code);
                int in,dn;
                FactorDataType::getDataLength(factor->nType,in,dn);
                if (dn==0)
                {
                    cmdString = cmdString.arg((int)(dataList.at(i)->value+0.5));
                }
                else
                {
                    cmdString = cmdString.arg(QString::number(dataList.at(i)->value,'f',4));//dn
                }
                cmdString = cmdString.arg(dataList.at(i)->dataFlag);
            }
        }
    }
    qDebug()<<"cmdString= "<<cmdString<<endl;

    if (cts!="")
    {
        cmdString+="&&";
     //   writeToUploadList(dt,cmdString);
//qDebug()<<"requestUploadRealData: "<<cmdString<<endl;
        requestUploadData(cmdString);
    }
}

//请求上传分钟数据。从 TStatisticalData 表中读取未上传的分钟数据，生成上传命令串，存入到上传列表中
bool BLocalService::requestUploadMimuteData()
{   
    if (sysConfig.SysDic.getDATA_UPLOAD_MINUTE_ENABLED()==false) //不上传分钟数据
        return true;

    QList<MStatisticalData> data;
    BStatisticalData bsd;
    bool r = bsd.outMinuteData(data);
    if (r)
    {
       writeToUploadList(data);
    }
    return r;
}

//请求上传小时数据。从 TStatisticalData 表中读取未上传的小时数据，生成上传命令串，存入到上传列表中
bool BLocalService::requestUploadHourData()
{
    if (sysConfig.SysDic.getDATA_UPLOAD_HOUR_ENABLED()==false) //不上传小时数据
        return true;

    QList<MStatisticalData> data;
    BStatisticalData bsd;
    bool r = bsd.outHourData(data);
    if (r && data.count()>0 )
    {
       writeToUploadList(data);
       return true;
    }
    else
       return false;
}

//请求上传日数据。从 TStatisticalData 表中读取未上传的日数据，生成上传命令串，存入到上传列表中
bool BLocalService::requestUploadDayData()
{
    if (sysConfig.SysDic.getDATA_UPLOAD_DAY_ENABLED()==false)  //不上传日数据
        return true;

 //qDebug()<<"requestUploadDayData..............DAY DAY DAY"<<endl;

    QList<MStatisticalData> data;
    BStatisticalData bsd;
    bool r = bsd.outDayData(data);
    if (r && data.count()>0 )
    {
       writeToUploadList(data);
       return true;
    }
    else
       return false;
}

//请求上传数据：上传数采仪开机时间
bool BLocalService::requestStartRunningTime()
{
    QString cmdString = this->getStartRunningTime();
    Task task;
    task.taskSource = TaskDirection::SELF_REPORT;
    PackingTools pt;
    DataPacketData dpd = pt.DataPacketDataBuilder(cmdString);
    task.dataPacket.data = dpd;
    task.dataPacket.data.SelfReportCmd = cmdString;
    CLog::getInstance()->log(cmdString);
    emit requestCloudService(task);
}

//请求上传数据：从上传数据表中读取记录上传
bool BLocalService::requestUploadData()
{
    bool r=true;
    /*
    if (sysConfig.ServerList.at(0)->status==1 || sysConfig.ServerList.at(1)->status==1)
    {
        QList<QString> key;
        QList< QList<QString> > * rows = new QList< QList<QString> >();
        int turn=0;
        do{
            r=bSqlite.find("TUploadDataList","1=1 limit 10",rows);
            if (r==false || rows->count()==0)
                break;

            for (int i=0;i<rows->count();i++)
            {
                Task task;
                task.taskSource = TaskDirection::SELF_REPORT;
                PackingTools pt;
                DataPacketData dpd = pt.DataPacketDataBuilder(rows->at(i).at(1));
                task.dataPacket.data = dpd;
                task.dataPacket.data.SelfReportCmd = rows->at(i).at(1);

                CLog::getInstance()->log(rows->at(i).at(1));
                emit requestCloudService(task);

                bSqlite.del("TUploadDataList","timestamp='"+rows->at(i).at(0)+"'");
            }
            turn ++;
            if (turn==5) break;
        }while(1);

        delete rows;
    }
    */
    return  r;
}

bool BLocalService::requestUploadData(QString cmdString)
{
    Task task;
    task.taskSource = TaskDirection::SELF_REPORT;
    PackingTools pt;
    DataPacketData dpd = pt.DataPacketDataBuilder(cmdString);
    task.dataPacket.data = dpd;
    task.dataPacket.data.SelfReportCmd = cmdString;
//qDebug()<<"LocalService send data: "<<cmdString<<endl;

    CLog::getInstance()->log(cmdString);

    emit requestCloudService(task);
return true;
}


CLog::CLog()
{

}

CLog *CLog::getInstance()
{
    static CLog clog;
    return &clog;
}

QMutex logMutex;
void CLog::log(QString cmdString)
{
//    QMutexLocker locker(&logMutex);

//    QDateTime dt= QDateTime::currentDateTime();

//    QFile file("log"+dt.toString("yyyyMMddHH")+".txt");
//    if(file.open(QFile::WriteOnly | QFile::Append ))
//    {
//        QTextStream out(&file);  //创建写入流
//        out << dt.toString("yyyy-MM-dd HH:mm:ss")<<" => "<<cmdString << "\r\n";//写入数据
//    }
//    file.close();
}
