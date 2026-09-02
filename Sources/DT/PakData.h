#ifndef PACKDATA_H
#define PACKDATA_H
#include <QString>
#include <QRegExp>
#include <QMap>
#include  <QDebug>
class PackData
{
public:
    PackData();
    //UnpackData(QString pack);
private:
    /*QString QN;
    QString ST;
    QString CN;
    QString PW;
    QString MN;
    QString Flag;
    QString PNUM;
    QString PNO;
    QString CP;
    */
    QMap<QString, QString> datamap;
public:
    void UnpackData(QString pack);
};

#endif // PACKDATA_H
