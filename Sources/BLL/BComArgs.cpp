#include "Headers/BLL/BComArgs.h"
#include "Headers/Model/MFactor.h"
#include "Headers/BLL/BFactor.h"
#include "Headers/Tools/JsonParse.h"

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QException>

QList<MComArgs*> BComArgs::ComArgsList;  //声明类成员static/共享变量。否则下面的成员函数不能使用
QMutex BComArgs::mutexComArgsList;   //声明类成员static/共享变量。否则下面的成员函数不能使用

BComArgs::BComArgs()
{
    this->tableName ="TComArgs";
}

BComArgs::~BComArgs()
{

}

int BComArgs::insert(MBase * rec)
{

}

int BComArgs::update(MBase * rec)
{

}

bool BComArgs::update(MComArgs *mca)
{
    QMap<QString, QString> where;
    where.insert("Cid",QString::number(mca->CId));

    QMap<QString, QString> data;
    data.insert("Status",QString::number((int)(mca->Status)));

    QString str="\'";
    str += mca->Args;
    str += "\'";

    data.insert("Args", str);
    return this->BBase::update(where,data);
}

int BComArgs::query(QString where, QList<MBase*> * list)
{
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r;
    r = sqlite->find(tableName,where,rows);
    for (int i=0;i<rows->size();i++)
    {
        MComArgs * mca = new MComArgs();
        QList<QString> rec = rows->at(i);
        mca->CId = ((QString)(rec.at(0))).toInt();
        setPortArgs(mca,rec.at(1),rec.at(2),rec.at(3),(COM_STATUS)(((QString)(rec.at(4))).toInt()),rec.at(5));
        list->append(mca);
    }
    delete rows;
    return r;
}

int BComArgs::query(QString where, QList<MComArgs*> * list)
{
    if (list!=nullptr)
    {
        for (int i=0;i<list->count();i++)
            delete  list->at(i);

        list->clear();
    }
    else
    {
        return 0;
    }

    return query(where, ( QList<MBase*> * ) list);
}

int BComArgs::queryDataComArgsList()
{
    return query("PortType!='N' ORDER By CId", & ComArgsList);
}

int BComArgs::queryDataComArgsList(QList<MComArgs *> *list)
{
    return query("PortType!='N' ORDER By CId", list);
}

QString BComArgs::getComIconFile( MComArgs * mca )
{
    enum PortStatus { UNUSED, CONNECTED, DISCONNECTED };
   /****************************************************************************************************************************/
        QString PortIconFile[8][3]={
               {"Images/rs232_b_32.png", "Images/rs232_g_32.png","Images/rs232_r_32.png"},
               {"Images/rs485_b_32.png", "Images/rs485_g_32.png","Images/rs485_r_32.png"},
               {"Images/ai_b_32.png", "Images/ai_g_32.png","Images/ai_r_32.png"},
               {"Images/switch_b_32.png", "Images/switch_g_32.png","Images/switch_r_32.png"},
               {"Images/r_b_32.png", "Images/r_g_32.png","Images/r_r_32.png"},
               {"Images/server_b_32.png", "Images/server_g_32.png","Images/server_r_32.png"},
               {"Images/ether_b_32.png", "Images/ether_g_32.png","Images/ether_r_32.png"},
               {"Images/4GDTU_b_32.png", "Images/4GDTU_g_32.png","Images/4GDTU_r_32.png"}
            };


    QString PortTypeList[] = {"RS232","RS485","A","S","R","SERVER","NETPORT","4G-DTU"};

    int portType=0;
    for ( ; portType<8;portType++)
    {
        if (PortTypeList[portType] == mca->PortType)
            break;
    }
    return PortIconFile[portType][mca->Status];
}

QList<QString> BComArgs::getUsingPort()
{
    QList<QString> portList;
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("PortName");
    where.insert("true","Status!=0");
    sqlite->find("TFactorDataList", key, where, &rows);

    for (int i=0;i<rows.count();i++)
    {
        portList.append(rows.at(i).at(0));
    }
    return portList;
}

QList<QString> BComArgs::getUsingDigitePort()
{
    QList<QString> portList;
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("PortName");
    where.insert("PortTypeName","'数字量通道'");
    where.insert("true","Status!=0");
    sqlite->find(this->tableName, key, where, &rows);

    for (int i=0;i<rows.count();i++)
    {
        portList.append(rows.at(i).at(0));
    }
    return portList;
}

bool BComArgs::setPortArgs(MComArgs * mca, QString portName, QString portType, QString portTypeName, COM_STATUS status, QString args)
{
    mca->PortName = portName;
    mca->PortType = portType;
    mca->PortTypeName = portTypeName;
    mca->Status =status;
    mca->Args = args;


    QJsonDocument jsonDocument = QJsonDocument::fromJson(mca->Args.toLocal8Bit().data());
    if (!(jsonDocument.isNull()))
    {
        QJsonObject comArgsJson = jsonDocument.object();
        mca->Speed = comArgsJson.value("Speed").toInt();
        mca->DataBit = comArgsJson.value("DataBit").toInt();
        mca->StopBit = comArgsJson.value("StopBit").toInt();
        mca->CheckBit = comArgsJson.value("CheckBit").toInt();

        mca->DataRangeMax = comArgsJson.value("DataRangeMax").toDouble();
        mca->DataRangeMin = comArgsJson.value("DataRangeMin").toDouble();
        mca->FactorDataRangeMax = comArgsJson.value("FactorDataRangeMax").toDouble();
        mca->FactorDataRangeMin = comArgsJson.value("FactorDataRangeMin").toDouble();
        mca->SignalType = (SIGNAL_TYPE) (comArgsJson.value("SignalType").toInt());

        mca->LogicType = (LOGIC_TYPE)(comArgsJson.value("LogicType").toInt());

        mca->StatusType = (CH_STATUS_TYPE)(comArgsJson.value("StatusType").toInt());

        QString factorArgsJsonStr = comArgsJson.value("FactorArgs").toString();
        jsonDocument = QJsonDocument::fromJson(factorArgsJsonStr.toLocal8Bit().data());
        if (!(jsonDocument.isNull()))
        {
            QJsonArray jsonArray = jsonDocument.array();
            if ( !(jsonArray.isEmpty()) && jsonArray.count()>0)
            {
                mca->ComFactorArgsList.clear();
                for (int j=0;j<jsonArray.size();j++)
                {
                    MFactor cfa;
                    QJsonObject jsonObj = jsonArray.at(j).toObject();
                    cfa.code = jsonObj.value("code").toString();
                    BFactor bf;
                    bf.getFactorInfoByCode(cfa.code, cfa);
                    cfa.stationAddr = jsonObj.value("stationAddr").toInt();
                    cfa.registerAddr = jsonObj.value("registerAddr").toInt();
                    cfa.cmdCode = jsonObj.value("cmdCode").toInt();
                    cfa.valueCount =jsonObj.value("valueCount").toInt();
                    cfa.warningValueMax = jsonObj.value("warningValueMax").toDouble();
                    cfa.warningValueMin = jsonObj.value("warningValueMin").toDouble();
                    cfa.plcDataType = jsonObj.value("plcDataType").toString();
                    cfa.deviceId = jsonObj.value("deviceId").toString();

                    cfa.signalType = (SIGNAL_TYPE)(jsonObj.value("signalType").toInt());
                    cfa.localTableName = jsonObj.value("localTableName").toString();
                    cfa.dataPortName = jsonObj.value("dataPortName").toString();
                    cfa.maxValue = jsonObj.value("maxValue").toDouble();
                    cfa.minValue = jsonObj.value("minValue").toDouble();

                    //初始化最大最小值和观测初值
                    if (cfa.maxValue == cfa.minValue)
                    {
                        cfa.maxValue = cfa.warningValueMax + (cfa.warningValueMax-cfa.warningValueMin)/4;
                        cfa.minValue = cfa.warningValueMin - (cfa.warningValueMax-cfa.warningValueMin)/4;

                    }
                    //cfa.value = cfa.warningValueMin + (cfa.warningValueMax-cfa.warningValueMin)/2;
                    //cfa.value = cfa.minValue;  //设置为观测的最小值
                    cfa.value = 0;//123456789; //设置成一个特殊值

                    cfa.valueTimestamp = QDateTime::currentDateTime();
                    mca->ComFactorArgsList.append(cfa);
                }
            }
        }
    }

    return true;
}

bool BComArgs::getNetConfig(QList<MComArgs> &netList)
{
    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("PortName");
    where.insert("PortType","'NETPORT'");
    bool r = sqlite->find(this->tableName, key, where, &rows);

    for (int i=0;i<rows.count();i++)
    {
        MComArgs mca;
        mca.CId = rows.at(i).at(0).toInt();
        mca.PortName = rows.at(i).at(1);
        mca.PortType = rows.at(i).at(2);
        mca.PortTypeName = rows.at(i).at(3);
        mca.Status = (COM_STATUS)(rows.at(i).at(4).toInt());
        mca.Args = rows.at(i).at(5);
        netList.append(mca);
    }
    return r;
}

bool BComArgs::getArgsByPortname(QString portName,  MComArgs &mca)
{
    QList< QList<QString> > rows;
    QString sql="SELECT * FROM '"+this->tableName+"' WHERE PortName='"+portName+"'";
    bool r = sqlite->find(sql, &rows);
    if (rows.count()>0)
    {
        QList<QString> rec =rows.at(0);
        mca.CId = rows.at(0).at(0).toInt();
        setPortArgs(&mca,rec.at(1),rec.at(2),rec.at(3),(COM_STATUS)(((QString)(rec.at(4))).toInt()),rec.at(5));
    }
    return r;
}

bool BComArgs::getUsingRelayPort(int &ch1, int &ch2)
{
    int c1,c2;
    c1=c2=-1;

    QList<QString> key;
    QMap<QString,QString> where;
    QList< QList<QString> > rows;
    key.append("PortName");
    key.append("Args");
    where.insert("PortType","'R'");
    where.insert("true","Status!=0");
    sqlite->find(this->tableName, key, where, &rows);

    for (int i=0;i<rows.count();i++)
    {
        QString args = rows.at(i).at(1);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(args.toLocal8Bit().data());
        if (!(jsonDocument.isNull()))
        {
            QJsonObject comArgsJson = jsonDocument.object();
            CH_STATUS_TYPE status = (CH_STATUS_TYPE)(comArgsJson.value("StatusType").toInt());
            if (status == CH_STATUS_TYPE::ALARM)
                c1 = rows.at(i).at(0).right(1).toInt();
            else  if (status == CH_STATUS_TYPE::THREE_COUNT)
                c2 = rows.at(i).at(0).right(1).toInt();
        }
    }
    if (c1==-1 || c2==-1)
        return false;
    else
    {
        ch1=c1;
        ch2=c2;
        return true;
    }
}



