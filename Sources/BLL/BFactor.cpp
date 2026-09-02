#include "Headers/BLL/BFactor.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutexLocker>

#include <Headers/BLL/BComArgs.h>

BFactor::BFactor()
{
    this->tableName = "TFactorCode";
}

BFactor::~BFactor()
{

}

int BFactor::insert(MBase * rec)
{

}

int BFactor::update(MBase * rec)
{

}

bool BFactor::update(QMap<QString, QString> where, QMap<QString, QString> data)
{
    return BBase::update(where,data);
}

int BFactor::query(QString where, QList<MBase*> * list)
{
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r = sqlite->find(tableName,where,rows);
    for (int i=0;i<rows->size();i++)
    {
        MFactor * mf = new MFactor();
        QList<QString> rec = rows->at(i);

        mf->code = rec.at(0);
        mf->chName = rec.at(1);
        mf->oldCode = rec.at(2);
        mf->unitC = rec.at(3);
        mf->unitE = rec.at(4);
        mf->nType = rec.at(5);
        mf->flag = rec.at(6).toInt();
        mf->args = rec.at(7);


        //若为水质分析仪，一开始不采样
        if (mf->code=="w01018" ||mf->code=="w01019" || mf->code=="w21001" || mf->code=="w21003" || mf->code=="w21011")
            mf->enabled = true;

        //解析 args
        QJsonDocument jsonDocument = QJsonDocument::fromJson(mf->args.toLocal8Bit().data());
        if (!(jsonDocument.isNull()))
        {
            QJsonObject jsonObj = jsonDocument.object();
            if (!(jsonObj.isEmpty()))
            {
                mf->dataPortName = jsonObj.value("dataPortName").toString(); //数据端口名称
                mf->stationAddr = jsonObj.value("stationAddr").toInt(); //从站地址
                mf->registerAddr = jsonObj.value("registerAddr").toInt(); //寄存器地址
                mf->cmdCode = jsonObj.value("cmdCode").toInt(); //命令码
                mf->valueCount = jsonObj.value("valueCount").toInt(); //取值个数
                mf->maxValue = jsonObj.value("maxValue").toDouble();  //图表中显示的最大值 或 量程上限
                mf->minValue = jsonObj.value("minValue").toDouble();  //图表中显示的最小值 或 量程下限
                mf->warningValueMax = jsonObj.value("warningValueMax").toDouble(); //最大告警值
                mf->warningValueMin = jsonObj.value("warningValueMin").toDouble(); //最小告警值
                mf->plcDataType = jsonObj.value("plcDataType").toString(); //PLC数据类型（大端、小端）
                mf->deviceId = jsonObj.value("deviceId").toString(); //设备编号
                mf->signalType = (SIGNAL_TYPE)(jsonObj.value("signalType").toInt());//监测因子 模拟量监测时的信号类型
                mf->localTableName = jsonObj.value("localTableName").toString();

                if (mf->flag==2) //特殊因子： 水质采样仪
                {
                    mf->enabled = false;  //默认不控制分析仪

                    mf->wqs.Interval = jsonObj.value("Interval").toInt();
                    mf->wqs.strProtocol = jsonObj.value("strProtocol").toString();
                    QString factorArgsJsonStr = jsonObj.value("Factors").toString();

                    jsonDocument = QJsonDocument::fromJson(factorArgsJsonStr.toLocal8Bit().data());
                    if (!(jsonDocument.isNull()))
                    {
                        QJsonArray jsonArray = jsonDocument.array();
                        if ( !(jsonArray.isEmpty()) && jsonArray.count()>0)
                        {
                            for (int j=0;j<jsonArray.size();j++)
                            {
                                mf->wqs.Factors.append(jsonArray.at(j).toString());
                            }
                        }
                    }

                }

            }
        }

        list->append(mf);
    }
    delete rows;
    return r;
}

int BFactor::query(QString where, QList<MFactor*> * list)
{
    if (list!=nullptr)
    {
        list->clear();
    }
    else
    {
        return 0;
    }

    return query(where, ( QList<MBase*> * ) list);
}

//根据代码返回名称
QString BFactor::getNameByCode(QString code)
{
    QList<MFactor*> list;

    if (code.length()==4 && (code.left(1)=="e" || code.left(1)=="g"))
        code = code + "xx";

    QString where ="Code='%01'";
    where = where.arg(code);

    query(where, &list);
    if (list.count()<=0)
    {
            return "";
    }
    QString name = list.at(0)->chName;

    for (int i =0;i<list.count();i++)
        delete list.at(i);
    return name;
}

QString BFactor::getDataTypeByCode(QString code)
{
    if (code.length()==4 && (code.left(1)=="e" || code.left(1)=="g"))
        code = code + "xx";

    QList<MFactor*> list;
    QString where ="Code='%01'";
    where = where.arg(code);

    query(where, &list);
    if (list.count()<=0)
    {
        return "";
    }
    QString nType = list.at(0)->nType;
    for (int i =0;i<list.count();i++)
        delete list.at(i);
    return nType;
}

bool BFactor::getFactorInfoByCode(QString code, MFactor & factor)
{
    if (code.length()==4 && (code.left(1)=="e" || code.left(1)=="g"))
        code = code + "xx";

    QList<MFactor*> list;
    QString where ="Code='%01'";
    where = where.arg(code);

    query(where, &list);
    if (list.count()<=0)
    {
        factor.chName="";
        return false;
    }

    factor = *(list.at(0));


    for (int i =0;i<list.count();i++)
        delete list.at(i);
    return true;
}

QString BFactor::getArgsByCode(QString code)
{
    if (code.length()==4 && (code.left(1)=="e" || code.left(1)=="g"))
        code = code + "xx";

    QList<MFactor*> list;
    QString where ="Code='%01'";
    where = where.arg(code);

    query(where, &list);
    if (list.count()<=0)
    {
        return "";
    }
    QString args = list.at(0)->args;
    for (int i =0;i<list.count();i++)
        delete list.at(i);
    return args;
}

bool BFactor::saveFactor(MFactor factor)
{
    QJsonObject jsonObj;
    jsonObj.insert("dataPortName",factor.dataPortName);
    jsonObj.insert("stationAddr",factor.stationAddr);
    jsonObj.insert("registerAddr",factor.registerAddr);
    jsonObj.insert("cmdCode",factor.cmdCode);
    jsonObj.insert("valueCount",factor.valueCount);
    jsonObj.insert("maxValue",factor.maxValue);
    jsonObj.insert("minValue",factor.minValue);
    jsonObj.insert("warningValueMax",factor.warningValueMax);
    jsonObj.insert("warningValueMin",factor.warningValueMin);
    jsonObj.insert("plcDataType",factor.plcDataType);
    jsonObj.insert("deviceId",factor.deviceId);
    jsonObj.insert("signalType",factor.signalType);
    jsonObj.insert("localTableName",factor.localTableName);

    if (factor.code=="wqs")
    {
        jsonObj.insert("Interval",factor.wqs.Interval);
        jsonObj.insert("strProtocol",factor.wqs.strProtocol);

        QJsonArray jsonArray;
        for (int i=0;i<factor.wqs.Factors.count();i++)
            jsonArray.append(factor.wqs.Factors.at(i));
        auto doc = QJsonDocument(jsonArray);
        QString argsStr = doc.toJson();
        jsonObj.insert("Factors",argsStr);
    }

    auto doc = QJsonDocument(jsonObj);
    factor.args = doc.toJson();

    QMap<QString,QString> where;
    QMap<QString,QString> data;

    QString code=factor.code;
    if (code.length()==4 && (code.left(1)=="e" || code.left(1)=="g"))
        code = code + "xx";

    where.insert("Code","'"+code+"'");
    data.insert("ChName","'"+factor.chName+"'");
    data.insert("OldCode","'"+factor.oldCode+"'");
    data.insert("UnitC","'"+factor.unitC+"'");
    data.insert("UnitE","'"+factor.unitE+"'");
    data.insert("NType","'"+factor.nType+"'");
    data.insert("flag",QString::number(factor.flag));
    data.insert("Args","'"+factor.args+"'");

    return sqlite->update(this->tableName,where,data);
}

bool BFactor::findFactor(QString factorCode, MFactor &factor)
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
