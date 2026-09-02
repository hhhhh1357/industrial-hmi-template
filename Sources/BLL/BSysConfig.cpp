#include "Headers/BLL/BSysConfig.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <Headers/Model/MComArgs.h>

#include <Headers/BLL/BComArgs.h>

extern Sqlite bSqlite;
extern QMutex mutexSqlite;

bool BSysConfig::getServerList(QList<Server*> &ServerList)
{
    ServerList.clear();
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r;
    r = bSqlite.find("TComArgs","PortType='SERVER'",rows);
    for (int i=0;i<rows->size();i++)
    {
        QString args = rows->at(i).at(5);
        Server * server=new Server();
        server->status = rows->at(i).at(4).toInt();
        server->serverName = rows->at(i).at(1);
        getServerArgs(args, server->ip, server->port,server->password,server->interval);
        ServerList.append(server);
    }
    delete rows;
    return r;
}

bool BSysConfig::getEthernetList(QList<Ethernet*> &EthernetList)
{
    EthernetList.clear();
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r;    

    r = bSqlite.find("TComArgs","PortType='NETPORT'",rows);
    for (int i=0;i<rows->size();i++)
    {
        QString args = rows->at(i).at(5);
        Ethernet *net=new Ethernet();
        net->status = rows->at(i).at(4).toInt();
        getEthernetArgs(args,net->portName,net->ip,net->mask,net->gate);
        EthernetList.append(net);
    }
    delete rows;
    return r;
}

BSysConfig::BSysConfig()
{
    getServerList(ServerList);
    getEthernetList(EthernetList);
    //dtu4G.enabled = get4GDtuEnabled();
    get4GDtu(dtu4G);
}

BSysConfig::~BSysConfig()
{
    for (int i=0;i<ServerList.count();i++)
        delete ServerList.at(i);
    for (int i=0;i<EthernetList.count();i++)
        delete EthernetList.at(i);
}

bool BSysConfig::saveEthernetArgs(Ethernet net)
{
    QJsonObject jsonObject;
    jsonObject.insert("PORTNAME",net.portName);
    jsonObject.insert("IPADDR",net.ip);
    jsonObject.insert("SUBMASK",net.mask);
    jsonObject.insert("NETWAY",net.gate);
    auto doc = QJsonDocument(jsonObject);

    MComArgs mca;
    BComArgs bca;
    bool r = bca.getArgsByPortname(net.portName,mca);
    if (r)
    {
        mca.Args = doc.toJson();
        mca.Status = (COM_STATUS)net.status;
        return bca.update(&mca);
    }
    else
        return false;
}

//**************20220606测试4G时屏蔽冯老师的代码****************//
bool BSysConfig::set4GDtuEnabled(bool enabled)
{
    dtu4G.enabled = enabled;
    return save4GDtu(dtu4G);
}

bool BSysConfig::save4GDtu(DTU4G dtu4g)
{
    MComArgs mca;
    BComArgs bca;
    bool r = bca.getArgsByPortname("4G-DTU-1",mca);
    if (r)
    {
        mca.Args = dtu4g.provider;
        mca.Status = dtu4g.enabled ? NORMAL : UNUSED;
        return bca.update(&mca);
    }
    return false;
}

//**************20220606测试4G时新添加的代码****************//
//bool BSysConfig::set4GDtuEnabled(bool enabled){

//}
//bool BSysConfig::save4GDtu(Ethernet net){
//    QJsonObject jsonObject;
//    jsonObject.insert("",net.provider);
//    auto doc = QJsonDocument(jsonObject);

//    MComArgs mca;
//    BComArgs bca;
//    bool r = bca.getArgsByPortname(net.portName,mca);
//    if (r)
//    {
//        mca.Args = doc.toJson();
//        mca.Status = (COM_STATUS)net.status;
//        return bca.update(&mca);
//    }
//    else
//        return false;
//}
//**********************************************************//

bool BSysConfig::getServerArgs(QString argsStr, QString &IP, int &PORT, QString &PASSWORD, int &INTERVAL)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(argsStr.toLocal8Bit().data());
    if (!(jsonDocument.isNull()))
    {
        QJsonObject comArgsJson = jsonDocument.object();
        IP = comArgsJson.value("IP").toString();
        PORT = comArgsJson.value("PORT").toInt();
        PASSWORD = comArgsJson.value("PASSWORD").toString();
        INTERVAL = comArgsJson.value("INTERVAL").toInt();
        return true;
    }
    else
    {
        return false;
    }
}

bool BSysConfig::getEthernetArgs(QString argsStr, QString &PORTNAME, QString &IPADDR, QString &SUBMASK, QString &NETWAY)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(argsStr.toLocal8Bit().data());
    if (!(jsonDocument.isNull()))
    {
        QJsonObject comArgsJson = jsonDocument.object();
        PORTNAME = comArgsJson.value("PORTNAME").toString();
        IPADDR = comArgsJson.value("IPADDR").toString();
        SUBMASK = comArgsJson.value("SUBMASK").toString();
        NETWAY = comArgsJson.value("NETWAY").toString();
        return true;
    }
    else
    {
        return false;
    }
}

bool BSysConfig::get4GDtu(DTU4G &dtu4g)
{
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r;

    r = bSqlite.find("TComArgs","PortType='4G-DTU'",rows);
    if (r&&rows->count()>0)
    {
        if (rows->at(0).at(4)=="1")
        {
            dtu4g.enabled = true;
            dtu4g.status=1;
        }
        else
        {
            dtu4g.enabled = false;
            dtu4g.status = 0;
        }
        dtu4g.provider=rows->at(0).at(5);
    }
    else
        dtu4g.enabled = false;;

    delete rows;
    return r;
}

bool BSysConfig::get4GDtuEnabled()
{
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r;

    r = bSqlite.find("TComArgs","PortType='4G-DTU'",rows);
    if (r&&rows->count()>0)
    {
        if (rows->at(0).at(4)=="1")
            r = true;
        else
            r = false;
    }
    else
        r = false;

    delete rows;
    return r;
}

bool BSysConfig::saveServerArgs(Server server)
{
    MComArgs mca;
    BComArgs bca;
    bool r = bca.getArgsByPortname(server.serverName,mca);

    QJsonObject jsonObject;
    jsonObject.insert("IP",server.ip);
    jsonObject.insert("PORT",server.port);
    jsonObject.insert("PASSWORD",server.password);
    jsonObject.insert("INTERVAL",server.interval);
    auto doc = QJsonDocument(jsonObject);
    mca.Args = doc.toJson();
    mca.Status = (COM_STATUS)server.status;
    return r && bca.update(&mca);
}

