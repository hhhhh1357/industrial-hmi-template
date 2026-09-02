#ifndef BSYSCONFIG_H
#define BSYSCONFIG_H

#include "BSysDictionary.h"
#include "Headers/DAL/dal.h"
#include <Headers/Model/MConfig.h>

class BSysConfig
{
public:
    BSysConfig();
    ~BSysConfig();

    SysDictionary SysDic;
    QList <Server*> ServerList;
    QList <Ethernet*> EthernetList;
    DTU4G dtu4G;

    bool getServerList(QList <Server*> &ServerList);
    bool getEthernetList(QList <Ethernet*> &EthernetList);

    bool getServerArgs(QString argsStr, QString &IP, int &PORT, QString &PASSWORD, int &INTERVAL);
    bool getEthernetArgs(QString argsStr, QString &PORTNAME, QString &IPADDR, QString &SUBMASK, QString &NETWAY);
    bool get4GDtu(DTU4G &dtu4g);
    bool get4GDtuEnabled();

    bool saveServerArgs(Server server);

    bool saveEthernetArgs(Ethernet net);
//**************20220606测试4G时屏蔽冯老师的代码****************//
    bool set4GDtuEnabled(bool enabled);
    bool save4GDtu(DTU4G dtu4g);

//*************20220606测试4G添加********************//
//    bool set4GDtuEnabled(bool enabled);
//    bool save4GDtu(Ethernet net);

//    /**********新添加4G运营商*********************/
//    bool set4GDtuEnabled(DTU4G dtu4G);

};

#endif // BSYSCONFIG_H
