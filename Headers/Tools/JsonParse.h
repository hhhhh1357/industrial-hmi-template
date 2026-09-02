#ifndef JSONPARSE_H
#define JSONPARSE_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QException>

class JsonParse
{
  public:
     static QJsonArray converJsonArray(QByteArray data);
     static QJsonObject converJson(QByteArray data);
     static QJsonObject converJson(QString data);
     static QByteArray converByteArray(QJsonObject object);
};


#endif // JSONPARSE_H
