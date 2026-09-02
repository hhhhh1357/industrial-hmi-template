// 检测的数据项头文件

#ifndef BDATAITEM_H
#define BDATAITEM_H

#include <QString>
#include <QDateTime>
#include "Headers/Model/MFactor.h"

class DataItem
{
public:
    MFactor * factor;

    DataItem(); //无参构造函数
    DataItem(MFactor * factor);

};

#endif // BDATAITEM_H
