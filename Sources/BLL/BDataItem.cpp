
#include "Headers/BLL/BDataItem.h"
#include <QString>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QException>

//无参构造函数
DataItem::DataItem()
{

}

DataItem::DataItem(MFactor * factor): DataItem()
{
    this->factor = factor ;
}

