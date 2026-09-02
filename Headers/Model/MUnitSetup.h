#ifndef UNIT_SETUP
#define UNIT_SETUP 1

#include <QString>
#include "MBase.h"

class MUnitSetup:public MBase
{
public:
    int id;
    QString code;
    QString name;
    QString unit;

    QString args;

};


#endif
