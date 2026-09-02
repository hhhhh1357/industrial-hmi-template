#include "Headers/BLL/initDB.h"


InitDB::InitDB()
{
   sqlite = new Sqlite("DAQT.db","DAQT","","");
}

InitDB::~InitDB()
{

}


void InitDB::creatDB()
{
    //createTable_UnitSetup();
    createTable_ComSetup();
    //createTable_CalcuSetup();

}

/**************create Table UnitSetup

*/

void InitDB::createTable_UnitSetup()
{
    QString table_name="UnitSetup";

    if (sqlite->isTableExist(table_name))
        return;

    QMap<QString,QString> table_data;
    table_data.insert("id","integer PRIMARY KEY autoincrement");
    table_data.insert("code","varchar(20)");
    table_data.insert("name","varchar(20)");
    table_data.insert("unit","varchar(20)");
    sqlite->create_table(table_name,table_data);
}


void InitDB::createTable_ComSetup()
{
    QString table_name="ComSetup";

    if (sqlite->isTableExist(table_name))
        return;

    QMap<QString,QString> table_data;
    table_data.insert("id","integer PRIMARY KEY autoincrement");
    table_data.insert("name","varchar(20)");
    table_data.insert("args","varchar(20)");
    sqlite->create_table(table_name,table_data);
}


void InitDB::createTable_CalcuSetup()
{
    QString table_name="CalcuSetup";

    if(sqlite->isTableExist((table_name)))
        return;

    QMap<QString,QString> table_data;
    table_data.insert("id","integer PRIMARY KEY autoincrement");
    table_data.insert("code","varchar(20)");
    table_data.insert("name","varchar(20)");
    table_data.insert("args","varchar(20)");
    sqlite->create_table(table_name,table_data);

}





