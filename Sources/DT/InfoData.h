#ifndef INFODATA_H
#define INFODATA_H
#include <QString>

class InfoData
{
public:
    InfoData(QString id, QString info);
    InfoData(const InfoData& obj);
    InfoData& operator=(const InfoData& obj);
    ~InfoData();
    QString Info; //现场端信息 数据类型不固定，暂时用字符形式表示
    QString InfoId;   //现场端信息编码

};



#endif // INFODATA_H
