#include "Headers/BLL/BUnitSetup.h"

BUnitSetup::BUnitSetup()
{
    tableName="UnitSetup";
}

int BUnitSetup::insert(MBase * rec)
{
    QMap<QString, QString> data;
    data.insert("code","\'"+((MUnitSetup*)rec)->code+"'");
    data.insert("unit","\'"+((MUnitSetup*)rec)->unit+"'");
    data.insert("name","\'"+((MUnitSetup*)rec)->name+"'");
    add(data);
}

int BUnitSetup::query(QString where, QList<MBase *> *list)
{
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r = sqlite->find(tableName,where,rows);
    for (int i=0;i<rows->size();i++)
    {
        MUnitSetup * mus = new MUnitSetup();
        QList<QString> rec = rows->at(i);
        mus->id = ((QString)(rec.at(0))).toInt();
        mus->code = rec.at(1);
        mus->name = rec.at(2);
        mus->unit = rec.at(3);
        list->append((MBase *)mus);
    }
    /*
    for (int i=0;i<rows->count();i++)
        delete &(rows->at(i));
        */
    delete rows;

    return r;
}

int BUnitSetup::update(MBase * rec)
{

    return 1;
}
