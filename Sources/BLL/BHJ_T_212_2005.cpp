#include "Headers/BLL/BHJ_T_212_2005.h"

BHJ_T_212_2005::BHJ_T_212_2005()
{
    this->tableName ="THJ_T_212_2005";
}

int BHJ_T_212_2005::query(QString where, QList<MBase*> * list)
{
    QList< QList<QString> > * rows = new QList< QList<QString> >();
    int r = sqlite->find(tableName,where,rows);
    for (int i=0;i<rows->size();i++)
    {
        MHJ_T_212_2005 * mhjt = new MHJ_T_212_2005();
        QList<QString> rec = rows->at(i);
        mhjt->id = ((QString)(rec.at(0))).toInt();
        mhjt->code = rec.at(1);
        mhjt->HName = rec.at(2);
        mhjt->HType = rec.at(3);
        mhjt->HUnitC = rec.at(4);
        mhjt->HItemC = rec.at(5);
        mhjt->HNType = rec.at(6);
        mhjt->HArgs =  rec.at(7);
        mhjt->HUnitE = rec.at(8);
        mhjt->HItemE = rec.at(9);
        list->append(mhjt);
    }
    for (int i=0;i<rows->count();i++)
        delete &(rows->at(i));
    delete rows;
    return r;
}

int BHJ_T_212_2005::query(QString where, QList<MHJ_T_212_2005*> * list)
{
    if (list!=nullptr)
    {
        list->clear();
    }
    else
    {
        list = new QList<MHJ_T_212_2005*>();
    }

    return query(where, ( QList<MBase*> * ) list);
}

bool BHJ_T_212_2005::getInfoByCode(MHJ_T_212_2005 *rec, QString code)
{
    if (rec==nullptr) return false;
    QList<MHJ_T_212_2005*> list;
    query("code='"+code+"'", &list);
    if (list.count()<=0) return false;
    rec->id = list.at(0)->id;
    rec->code = code;
    rec->HArgs = list.at(0)->HArgs;
    rec->HName = list.at(0)->HName;
    rec->HType = list.at(0)->HType;
    rec->HItemC = list.at(0)->HItemC;
    rec->HItemE = list.at(0)->HItemE;
    rec->HNType = list.at(0)->HNType;
    rec->HUnitC = list.at(0)->HUnitC;
    rec->HUnitE = list.at(0)->HUnitE;

    for (int i=0;i<list.count();i++)
        delete list.at(i);

    return true;
}

QString BHJ_T_212_2005::getArgsByCode(QString code)
{
    QList<MHJ_T_212_2005*> list;
    query("code='"+code+"'", &list);
    if (list.count()<=0) return "";
    QString args = list.at(0)->HArgs;

    for (int i=0;i<list.count();i++)
        delete list.at(i);

    return args;
}

bool BHJ_T_212_2005::updateArgsByCode(QString args, QString code)
{
    QMap<QString, QString> where;
    QMap<QString, QString> data;
    where.insert("code","'"+code+"'");
    data.insert("HArgs","'"+args+"'");

    int r = updata(where, data);
    if (r==1) return true;
    else return false;
}


DataItem BHJ_T_212_2005::getDataItemByCode(QString code)
{

    DataItem di ;
    return di;
}

int BHJ_T_212_2005::insert(MBase * rec)
{

}

int BHJ_T_212_2005::update(MBase * rec)
{

}

