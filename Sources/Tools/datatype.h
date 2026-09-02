#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>

class FactorDataType
{
public:
    FactorDataType();
    static bool getDataLength(QString dataType, int &in,int &dn);
};

class RandomData
{
public:
   static float getRadomData(double max, double min);
};

#endif // DATATYPE_H
