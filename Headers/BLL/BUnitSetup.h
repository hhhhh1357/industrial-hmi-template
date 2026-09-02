#ifndef BUNITSETUP_H
#define BUNITSETUP_H

#include "Headers/DAL/dal.h"
#include "Headers/BLL/BBase.h"
#include "Headers/Model/MUnitSetup.h"



class BUnitSetup: public BBase
{

public:
    BUnitSetup();

    int virtual insert(MBase * rec);
    int virtual update(MBase * rec) ;
    int virtual query(QString where, QList<MBase*> * list);
};

#endif // BUNITSETUP_H

