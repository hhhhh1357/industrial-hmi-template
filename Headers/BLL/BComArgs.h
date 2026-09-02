#ifndef BCOMARGS_H
#define BCOMARGS_H

#include <QString>
#include <QMutex>
#include <QMutexLocker>

#include "BBase.h"
#include "BFactor.h"
#include "Headers/Model/MComArgs.h"

class BComArgs:public BBase
{

public:
    BComArgs();
    ~BComArgs();

    int virtual insert(MBase * rec);
    int virtual update(MBase * rec) ;
    int virtual query(QString where, QList<MBase*> * list);
    int query(QString where, QList<MComArgs*> * list);

    bool update(MComArgs * mca);
    int queryDataComArgsList();    
    int queryDataComArgsList(QList<MComArgs*> * list);

    static QString getComIconFile(MComArgs * mca);

    static QList<MComArgs*> ComArgsList; // 定义静态成员，实现共享数据
    static QMutex mutexComArgsList; //端口列表的互斥锁

    QList<MComArgs*> ComArgsListNew; //当前数据库中最新的配置

    QList <QString> getUsingPort();
    QList <QString> getUsingDigitePort();

    bool setPortArgs(MComArgs * mca, QString portName, QString portType, QString portTypeName, COM_STATUS status, QString args);

    bool getNetConfig(QList<MComArgs> &netList);
    bool getArgsByPortname(QString portName, MComArgs &mca);

    bool getUsingRelayPort(int &ch1, int &ch2);

};

#endif // BCOMARGS_H
