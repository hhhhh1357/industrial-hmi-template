#include"Headers/BLL/BCalcSetup.h"

#include"Headers/BLL/BBase.h"

BCalcSetup::BCalcSetup()
{
    tableName="CalcSetup";
}


int BCalcSetup::insert(MBase * rec)
{

    QMap<QString, QString> data;
    data.insert("code","\'"+((MCalcSetup*)rec)->code+"'");
    data.insert("args","\'"+((MCalcSetup*)rec)->args+"'");
    data.insert("name","\'"+((MCalcSetup*)rec)->name+"'");
    add(data);
}


int BCalcSetup::query(QString where, QList<MBase *> *list)
{
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r = sqlite->find(tableName,where,rows);
    for (int i=0;i<rows->size();i++)
    {
        MCalcSetup * mcs = new MCalcSetup();
        QList<QString> rec = rows->at(i);
        mcs->id = ((QString)(rec.at(0))).toInt();
        mcs->code = rec.at(1);
        mcs->name = rec.at(2);
        mcs->args = rec.at(3);
        list->append((MBase*)mcs);
    }
    /*
    for (int i=0;i<rows->count();i++)
        delete &(rows->at(i));
        */

    delete rows;
    return r;
}
/**************************测试Update********************/
int BCalcSetup::update(MBase * rec)
{
    QMap<QString, QString> where;
    QMap<QString, QString> data;

    where.insert("code", "\'"+ ((MCalcSetup *)rec)->code +"\'");
    where.insert("name","\'"+((MCalcSetup *)rec)->name+"\'");

    data.insert("name","\'"+((MCalcSetup *)rec)->name+"\'");
    data.insert("args","\'"+((MCalcSetup *)rec)->args+"\'");

    bool r = sqlite->update(tableName, where, data);
    QString e = sqlite->getError();
    int i=0;i++;
}


















