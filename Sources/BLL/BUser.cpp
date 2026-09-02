#include "Headers/BLL/BUser.h"
#include "Headers/DAL/dal.h"


BUser::BUser()
{
    tableName = "TUser";
}


bool BUser::login(MUser user)
{
    return this->login(user.uName,user.uPassword);
}


bool BUser::login(QString uName, QString uPassword)
{
    QString where="uname='"+uName+"' and upassword='"+uPassword+"'";
    if (sqlite->recordCount(tableName,where)>0)
        return true;
    else
        return false;
}

bool BUser::setPassword(QString uName, QString uPassword)
{
    QMap<QString, QString> where;
    QMap<QString, QString> data;
    where.insert("uname","'"+uName+"'");
    data.insert("upassword","'"+uPassword+"'");
    return this->update(where,data);
}

bool BUser::update(QMap<QString, QString> where, QMap<QString, QString> data)
{
    return BBase::update(where,data);
}
