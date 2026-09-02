#ifndef MCALCSETUP
#define MCALCSETUP

#include <QString>
#include "MBase.h"

class MCalcSetup:public MBase
{
public:
    int id;
    QString code;
    QString name;
    QString args;
};

#endif // MCALCSETUP

