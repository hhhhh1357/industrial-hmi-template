#ifndef INITDB
#define INITDB

#include "Headers/DAL/dal.h"

class InitDB
{
    Sqlite * sqlite;

public:
    InitDB();
    ~InitDB();

    void creatDB();
    void createTable_UnitSetup();
    void createTable_ComSetup();

    void createTable_CalcuSetup();
};



#endif
