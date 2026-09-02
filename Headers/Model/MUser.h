#ifndef MUSER_H
#define MUSER_H

#include <QString>
#include "MBase.h"

class MUser:public MBase
{
public:
    int uid;
    QString uName;
    QString uPassword;
};


#endif // MUSER_H
