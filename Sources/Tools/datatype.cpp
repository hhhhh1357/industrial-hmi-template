#include "datatype.h"

#include <QDateTime>

FactorDataType::FactorDataType()
{

}

bool FactorDataType::getDataLength(QString dataType, int &in, int &dn)
{
    if (dataType.left(1)!="N")
        return false;

    int p = dataType.indexOf('.');
    if (p>-1)
    {
        in=dataType.left(p).right(p-1).toInt();
        dn=dataType.right(dataType.length()-p-1).toInt();
    }
    else
    {
        in=dataType.right(dataType.length()-1).toInt();
        dn=0;
    }

    return true;
}

float RandomData::getRadomData(double max, double min)
{
    if (max==min)
        return max;

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int n = qrand() % ((int)(max-min));

    float v = min+n;

    return v;
}
