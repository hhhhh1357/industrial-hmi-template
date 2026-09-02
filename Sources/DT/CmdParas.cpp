#include "CmdParas.h"

CmdParas::CmdParas(QObject *parent) : QObject(parent)
{

}


CmdParas::CmdParas(const CmdParas &temp,QObject *parent ):QObject(parent)
{
    SystemTime = temp.SystemTime;  //系统时间
    QnRtn = temp.QnRtn;    //请求回应
    ExeRtn = temp.ExeRtn;   //执行结果回应
    RtdInterval = temp.RtdInterval;   //实时采样数据上报
    MinInterval = temp.MinInterval;   //分钟数据上报间隔
    RestartTime = temp.RestartTime;   //数采仪开机时间

    //this->polDataList = temp.polDataList;  //这样赋值可能会出问题

    //每个污染因子对应的各种数据存入polDataList中
    for(int i=0; i<temp.polDataList.size(); i++){
        this->polDataList.append(temp.polDataList.at(i));
    }
    //每个设备对应的各种数据存入sbDataList中
    foreach (const SBData &sbData, temp.sbDataList) {
        this->sbDataList.append(sbData);
    }
    //每个现场端信息对应的各种数据存入infoDataList中
    foreach(const InfoData &infoData, temp.infoDataList){//现场端信息 数据类型不固定，暂时用字符形式表示//现场端信息编码
        this->infoDataList.append(infoData);
    }

    BeginTime = temp.BeginTime;//开始时间
    EndTime = temp.EndTime;  //截止时间
    DataTime = temp.DataTime; //数据时间信息
    NewPW = temp.NewPW;    //新密码
    OverTime = temp.OverTime; //超时时间
    ReCount = temp.ReCount;  //重发次数
    VaseNo = temp.VaseNo;   //采样瓶编号
    CstartTime = temp.CstartTime;//设备采样起始时间
    Ctime = temp.Ctime;    //采样周期
    Stime = temp.Stime;    //出样时间

    SN = temp.SN; //在线监控（监测）仪器仪表编码


}

CmdParas &CmdParas::operator=(const CmdParas &temp)
{
    SystemTime = temp.SystemTime;  //系统时间
    QnRtn = temp.QnRtn;    //请求回应
    ExeRtn = temp.ExeRtn;   //执行结果回应
    RtdInterval = temp.RtdInterval;   //实时采样数据上报
    MinInterval = temp.MinInterval;   //分钟数据上报间隔
    RestartTime = temp.RestartTime;   //数采仪开机时间

    //每个污染因子对应的各种数据存入polDataList中
    for(int i=0; i<temp.polDataList.size(); i++){
        this->polDataList.append(temp.polDataList.at(i));
    }
    //每个设备对应的各种数据存入sbDataList中
    foreach (const SBData &sbData, temp.sbDataList) {
        this->sbDataList.append(sbData);
    }
    //每个现场端信息对应的各种数据存入infoDataList中
    foreach(const InfoData &infoData, temp.infoDataList){//现场端信息 数据类型不固定，暂时用字符形式表示//现场端信息编码
        this->infoDataList.append(infoData);
    }

    BeginTime = temp.BeginTime;//开始时间
    EndTime = temp.EndTime;  //截止时间
    DataTime = temp.DataTime; //数据时间信息
    NewPW = temp.NewPW;    //新密码
    OverTime = temp.OverTime; //超时时间
    ReCount = temp.ReCount;  //重发次数
    VaseNo = temp.VaseNo;   //采样瓶编号
    CstartTime = temp.CstartTime;//设备采样起始时间
    Ctime = temp.Ctime;    //采样周期
    Stime = temp.Stime;    //出样时间

    SN = temp.SN; //在线监控（监测）仪器仪表编码

    return *this;
}
