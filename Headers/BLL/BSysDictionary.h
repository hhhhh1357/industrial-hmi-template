#ifndef BSYSDICTIONARY_H
#define BSYSDICTIONARY_H

#include <QString>
#include <QMap>
#include "Headers/DAL/dal.h"

extern Sqlite bSqlite;

class SysDictionaryKV
{
public:
    QString key;
    QString value;
    QString dataType;
    QString memo;
};

class SysDictionary
{
private:
    SysDictionaryKV MN;        //设备号
    SysDictionaryKV ST;        //站点系统编码
    SysDictionaryKV TIMEOUT;       //超时时间(秒)
    SysDictionaryKV REPEAT;        //最大重发次数
    SysDictionaryKV STORAGE_TIME;  //本地数据保存时长(小时)
    SysDictionaryKV INTERVAL;           //数据采集间隔(秒)

    SysDictionaryKV DATA_UPLOAD_REAL;   //实时数据上报时间间隔（秒）
    SysDictionaryKV DATA_UPLOAD_MINUTE; //分钟数据上报时间间隔（秒）
    SysDictionaryKV DATA_UPLOAD_HOUR;   //小时数据上报时间间隔（秒）
    SysDictionaryKV DATA_UPLOAD_DAY;    //日数据上报时间（整点）

    SysDictionaryKV DATA_UPLOAD_REAL_ENABLED;//实时数据是否上报
    SysDictionaryKV DATA_UPLOAD_MINUTE_ENABLED;//实时数据是否上报
    SysDictionaryKV DATA_UPLOAD_HOUR_ENABLED;//实时数据是否上报
    SysDictionaryKV DATA_UPLOAD_DAY_ENABLED;//实时数据是否上报

    SysDictionaryKV DOG_TIMER; //软件看门狗巡逻时间

    SysDictionaryKV SCREEN_SAVE_TIME; //屏保时间

    SysDictionaryKV TABLE_LENGTH;        //默认数据表长度

    SysDictionaryKV PING_SERVERS;  //互联网连通测试服务器。一个Json串

    QString tableName;
    Sqlite * sqlite;



public:
    SysDictionary();

    void getSysDictionary();    
//    bool putSysDictionary(QString MN, QString ST, int TIMEOUT, int REPEAT, int STORAGE_TIME, int INTERVAL,
//                          int DATA_UPLOAD_REAL, int DATA_UPLOAD_MINUTE, int DATA_UPLOAD_HOUR, bool DATA_UPLOAD_REAL_ENABLED);

    QString getMN();
    bool putMN(QString MN);

    QString getST();
    bool putST(QString ST);

    int getTIMEOUT();
    bool putTIMEOUT(int TIMEOUT);

    int getREPEAT();
    bool putREPEAT(int REPEAT);

    int getSTORAGE_TIME();
    bool putSTORAGE_TIME(int STORAGE_TIME);

    int getINTERVAL();
    bool putINTERVAL(int INTERVAL);

    int getDATA_UPLOAD_REAL();
    bool putDATA_UPLOAD_REAL(int DATA_UPLOAD_REAL);

    int getDATA_UPLOAD_MINUTE();
    bool putDATA_UPLOAD_MINUTE(int DATA_UPLOAD_MINUTE);

    int getDATA_UPLOAD_HOUR();
    bool putDATA_UPLOAD_HOUR(int DATA_UPLOAD_HOUR);

    int getDATA_UPLOAD_DAY();
    bool putDATA_UPLOAD_DAY(int DATA_UPLOAD_DAY);

    bool getDATA_UPLOAD_REAL_ENABLED();
    bool putDATA_UPLOAD_REAL_ENABLED(bool enabled);

    bool getDATA_UPLOAD_MINUTE_ENABLED();
    bool putDATA_UPLOAD_MINUTE_ENABLED(bool enabled);

    bool getDATA_UPLOAD_HOUR_ENABLED();
    bool putDATA_UPLOAD_HOUR_ENABLED(bool enabled);

    bool getDATA_UPLOAD_DAY_ENABLED();
    bool putDATA_UPLOAD_DAY_ENABLED(bool enabled);

    int getTABLE_LENGTH();
    bool putTABLE_LENGTH(int TableLength);

    int getDOG_TIMER();
    bool putDOG_TIMER(int DOG_TIMER);

    int getSCREEN_SAVE_TIME();
    bool putSCREEN_SAVE_TIME(int SCREEN_SAVE_TIME);

    QString getPING_SERVERS();
    bool putPING_SERVERS(QString jsonStr);
    int getPING_SERVERS_Interval();
    QList<QString> getPING_SERVERS_List();
};

#endif // BSYSDICTIONARY_H
