#include "Headers/Tools/JsonParse.h"

// 将字符数组 QByteArray 转换成Json数组 QJsonArray
QJsonArray JsonParse::converJsonArray(QByteArray data)
{
    QJsonArray object;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(!doc.isNull()){
        object = doc.array();
    }
    return object;
}

// 将字符数组 QByteArray 转换成Json对象 QJsonObject
QJsonObject JsonParse::converJson(QByteArray data)
{
    QJsonObject object;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(!doc.isNull()){
        object = doc.object();
    }
    return object;
}

// 将字符串 QString 转换成Json对象 QJsonObject
QJsonObject JsonParse::converJson(QString data)
{
    QJsonObject object;
    QJsonDocument doc = QJsonDocument::fromJson(data.toLatin1());
    if(!doc.isNull()){
        object = doc.object();
    }
    return object;
}

// 将Json对象 QJsonObject 转换成字符数组 QByteArray
QByteArray JsonParse::converByteArray(QJsonObject object)
{
    QJsonObject json;
    QJsonDocument doc = QJsonDocument(object);
    return doc.toJson();
}
