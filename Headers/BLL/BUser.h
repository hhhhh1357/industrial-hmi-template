#ifndef BUSER_H
#define BUSER_H

#include "BBase.h"
#include "Headers/Model/MUser.h"

class BUser: public BBase
{
public:
    BUser();
    bool login(MUser user);
    bool login(QString uName, QString uPassword);
    bool setPassword(QString uName, QString uPassword);

private:
    int insert(MBase * rec) {};
    int update(MBase * rec) {};
    bool update(QMap<QString, QString> where, QMap<QString, QString> data);
    int query(QString where, QList<MBase*> * list){};

    int del(int id )  {};
    int del(QString code){}
    bool isRecordExist(QString code){};
    bool isRecordExist(int id){};

};


#endif // BUSER_H
