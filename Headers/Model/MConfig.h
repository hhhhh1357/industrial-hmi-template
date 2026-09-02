#ifndef MCONFIG_H
#define MCONFIG_H

#include <QString>

class DTU4G
{
public:
//*********************20220606测试4G保存时屏蔽************************//
    bool enabled=true; //是否使用
    //QString portName;//网口名称//*********************2022-5-22新添加*************************//
    int status=2;  //状态
    QString provider;//运营商 ChinaTelecom、ChinaMobile、ChinaUnicom /
};

class Ethernet
{
public:
    QString portName; //网口名称
    QString ip;   //IP地址
    QString mask; //子网掩码
    QString gate; //网关
    int status;   //状态
//******************20220606测试4G保存时添加**************************//
//    bool enabled=true; //是否使用
//    //QString portName;//网口名称//*********************2022-5-22新添加*************************//
//    int Status=2;  //状态
//    QString provider;//运营商 ChinaTelecom、ChinaMobile、ChinaUnicom /
};

class Server
{
public:
    QString serverName;
    QString ip; //IP地址
    int port;   //端口号
    QString password;//密码
    int interval; //上传数据时间周期
    int status;   //状态
};





#endif // MCONFIG_H
