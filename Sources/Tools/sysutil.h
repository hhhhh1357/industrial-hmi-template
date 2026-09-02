#ifndef SYSUTIL_H
#define SYSUTIL_H

#ifdef __cplusplus
extern "C" {
#endif
#include <sys/time.h>
#ifdef __cplusplus
}
#endif

#include <QDateTime>
#include <QVector>
#include "Sources/Tools/key.h"
#include "Sources/DT/uart.h"

#include <Headers/BLL/BSysConfig.h>
//#include <QtXlsx>

class SysUtil
{
public:
    SysUtil();

    static bool setDatetime(QDateTime &timestamp);
    static bool exportExcel(QString fileName, QList<QString> colName, QList<QVector<double> * > &data );
    static bool exportCSV(QString fileName, QList<QString> colName, QList<QVector<double> * > &data );
    static bool getSDcardStatus();
    static bool getUSBStatus();


    //设置网口
    static bool setEthernet(int ch, QString ip, QString mask, QString gateway);
    static bool setEthernet(QString portName, QString ip, QString mask, QString gateway);
    //4G拨号联网
    static bool dial4GDtu(QString provider);
    //关闭指定的网口
    static void closeNetwork(int ch);
    static void closeNetwork(QString portName);

    //设置通讯网络：是启用以太网 还是启用 4G
    static bool SetNetWork(BSysConfig & sysConfig);

    //获取以太网状态
    static bool getEthernetStatus(int ch);
    static bool getEthernetStatus(QString portName);

    //获取4G 状态
    static bool get4gDtuStatus();

    //判断开关量状态
    static bool getSwitchStatus(KEY key);
    static bool getSwitchStatus(QString portName);
    //判断继电器状态
    static bool getRelayStatus(RELAYER_CHANNEL ch);
    static bool getRelayStatus(QString portName);


};

#endif // SYSUTIL_H
