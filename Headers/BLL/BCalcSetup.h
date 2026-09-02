#ifndef CALCUSETUP
#define CALCUSETUP


#include "Headers/DAL/dal.h"
#include "Headers/BLL/BBase.h"
#include "Headers/Model/MCalcSetup.h"

class BCalcSetup : public BBase
{     
public:
    BCalcSetup();
    int virtual insert(MBase * rec);
    int virtual update(MBase * rec) ;
    int virtual query(QString where, QList<MBase*> * list);
};





























#endif // CALCUSETUP

