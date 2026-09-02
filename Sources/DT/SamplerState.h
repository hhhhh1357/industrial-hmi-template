#ifndef SAMPLERSTATE_H
#define SAMPLERSTATE_H
#include <QString>
#include <QMap>
#include "uart.h"
#include "DataAlign.h"
#include <QDebug>
#include <QThread>

typedef enum{
    FLOAT_DATA = 1,
    DATE_DATA = 2,
    INT_DATA =3
}DataType;

struct Instrument_Info{
    DataType datatype;
    QString RegAddress;
};
static struct Instrument_Info instrument_info[]={
{   //测量量程的地址和数据类型
    .datatype = INT_DATA,
    .RegAddress = "1520",
},
{   //测量精度的数据地址和类型
    .datatype =INT_DATA,
    .RegAddress =  "1521",

},
{   //测量间隔的地址和数据类型
    .datatype =INT_DATA,
    .RegAddress = "1522",

},
{   //消解温度的地址和数据类型
    .datatype =INT_DATA,
    .RegAddress = "1522",

},
{   //消解时长的地址和数据类型
    .datatype =INT_DATA,
    .RegAddress = "1524",

},
{   //曲线斜率的地址和数据类型
    .datatype =FLOAT_DATA,
    .RegAddress = "1531",

},
{   //曲线截距的地址和数据类型
    .datatype =FLOAT_DATA,
    .RegAddress = "1533",

},
{   //测量检出限的地址和数据类型
    .datatype =FLOAT_DATA,
    .RegAddress = "1535",

},
{   //测量信号值的地址和数据类型
    .datatype =FLOAT_DATA,
    .RegAddress =  "1537",

},
{   //线性相关系数的地址和数据类型
    .datatype = FLOAT_DATA,
    .RegAddress = "1539",

},
{   //二次多项式系数的地址和数据类型
    .datatype= FLOAT_DATA,
    .RegAddress = "1541",

},
{   //标样校准时间
    .datatype= DATE_DATA,
    .RegAddress = "1543",

}
};

static const QMap<QString, struct Instrument_Info>InfoMap={
{"i13001",instrument_info[0]},
{"i13002",instrument_info[1]},
{"i13003",instrument_info[2]},
{"i13004",instrument_info[3]},
{"i13005",instrument_info[4]},
{"i13007",instrument_info[5]},
{"i13008",instrument_info[6]},
{"i13009",instrument_info[7]},
{"i13010",instrument_info[8]},
{"i13011",instrument_info[9]},
{"i13012",instrument_info[10]},
{"i13009",instrument_info[11]},
};//键(InfoId)-值（数值）

class SamplerState:public QThread
{

public:
    QMap<QString, QString> sample_info;
    QString UpCYQ;
    unsigned char buf[120]={0};//采样状态数据
    unsigned char buff[113]={0};//采样器留样数据
    struct sample_status {
        QString i42001[2] = {"待机","运行"};
        QString i42002[5] = {"无报警","采样失败","供样失败","留样失败","温度报警"};
        QString i42003[5] = {"定时采样","时间比例采样","流量等比采样","流量跟踪采样","其他"};
        QString i42101[2] = {"关","开"};
        QString i42102[3] = {"正常","故障","关门异常"};

        QString i43101[2] = {"门打开","门关闭"};
        QString i43102[5] = {"运行","停机","故障","维护","报警"};

        QString DataFlag[11]={"N","T","L","P","D","F","B","Z","S","M","hd"};
        QString RunLog[100] = {"数据丢失","上电初始化程序","上电冷却消解液","上电后一次清洗",
                              "上电后二次清洗","排空管路","预采样","外加泵循环",
                              "采样","试样返回","采稀释水",
                              "采稀释水(2)"," "," ","采试剂1",
                              "采蒸馏水","采试剂二(1)","采试剂二(2)",
                              "混合","加热消解","冷却","静置比色1",
                              "排反应液","清洗1","清洗2","清洗3",
                              "计算","等待下一次","校准结束","无水样,等待下次启动","润洗",
                              "第二次采样","抽混合液(1)",
                              "抽混合液(2)","抽最后稀释液及定位","静置比色2","滴定前升温",
                              "静置反应"," "," "," "," "," "," "," ",
                              "清洗结束"," ","超标信号输出"," "," ","首次启动排空","更新注射器试剂一",
                              "采洗滴定管蒸馏水","滴定管推出少量",
                              "抽出1杯到计量管","滴定液返回少量","排出洗液",
                              "滴定","首次启动之排空注射器",
                               " "," "," "," "," "," "," "," "," "," ",
                               " "," "," "," "," "," "," "," "," "," ",
                               " "," "," "," "," "," "," "," "," "," ",
                               " "," "," "," "," "," "," "," "," "," ",
                               "待启动"};

        QString DeviceStatus[45] = {" ","启动测量","标样核查","零点检查","跨度核查","空白测试","平行样测试","加标回收","自动空白校准","自动标1校准","自动标2校准","初始化(清洗)","停止测试","仪器重启","校时","模式设置","测量间隔设置","零点核查间隔设置","跨度核查间隔设置","标样核查间隔设置","手动两点标定转维护之零标","手动两点标定转维护之量标","手动两点标定转维护之零标","手动两点标定转维护之量标","维护测试","C1标定","C2标定","C3标定","C4标定","C5标定","手动空白校准","手动标样校准","核查不合格之标1校准","核查不合格之标2校准","手动三点标定转维护之零点标定","手动三点标定转维护之标1标定","手动三点标定转维护之标2标定","手动三点标定转维护之零点标定","手动三点标定转水样之零点标定","手动三点标定转水样之标1标定","手动三点标定转水样之标2标定","远程启动单次测试","启动单台仪表空白校准","启动单台仪表标样校准","启动单台仪表标液核查"};
        QString WarningStatus[25] = {"无警告","缺试剂告警","缺水样警","缺蒸馏水告警","缺标液告警","仪表漏液告警","标定异常告警","超量程告警","加热异常","低试剂预警","超上限告警","超下限告警","滴定异常告警","电极异常告警","量程切换告警","参数设置告警","参数设置告警","参数设置告警","浊度光异常","浊度光异常","浊度光异常","稀释液定位异常","液位信号异常","比色信号低"};

    };
    struct sample_status status;

private:
    QString DataFormatAnalysis(DataAlign dataformat, unsigned char* data);
    void DataFormatAnalysis(DataAlign dataformat, unsigned char* data,float floatdata);



public:
    SamplerState();
    ~SamplerState();
    bool MonitorIndex(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,DataAlign dataformat, QMap<QString, QString> &resultMap);
    bool readWQSDataInfo(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, QMap<QString, QString> &resultMap);
    bool readWQSDataInfo_new(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, QMap<QString, QString> &resultMap);
    bool readAnalysisMeterInfo(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QMap<QString, QString> &resultMap, DataAlign dataformat);
    bool dev(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, DataAlign dataformat, QMap<QString, QString> &resultMap);
    //怡杉专用因子
    QString YSENVIR(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, DataAlign dataformat);
public:
    //获取采样状态数据并存放在buf里面
    bool getWQSData(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    bool getWQSData_new(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    void WQSDataInfo(QMap<QString, QString> &sample_info);//解析buf里的采样状态信息
    void WQSDataInfo_new(QMap<QString, QString> &sample_info);//解析buf里的采样状态信息

    //获取分析仪，并存放到buff里面
    void StateExtract(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    void Get_StateExtract_Info(QMap<QString, QString> &sample_info, DataAlign dataformat);//解析buff里面的采样器留样数据
    //数采仪控制水质采样器
    void StartStruc(URT_PORT port, unsigned short reg_address,SERIAL_PORT_CONFIG config);
    //数采仪发水质采样器启停命令
    void SamplerStartStop(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, QString cmdd);
    //数采仪发水质采样器留样指令
    void OverReSample(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString cmdd);
    //水质采样器供水
    QString WaterSupply(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    //分析仪启停
    void AnalyzerStartStop(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString cmdd);
    //上传监测指标核查
    bool MonitorIndexCheck(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    void MonitorIndexCheck_data(DataAlign dataformat, QMap<QString, QString> &sample_info);

    //上传设备唯一标识
    bool DevFlag(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    void DevFlag_Date(DataAlign dataformat, QMap<QString, QString> &sample_info);

    //提取出样时间
    bool SamplerTime(URT_PORT port, unsigned char salve_address, SERIAL_PORT_CONFIG config);
    //参数说明：读写端口，来自云端的Info_Id, 来自云端的值，从站地址，端口配置信息
    void Set_instrument_Info(URT_PORT port, QString Info_Id, QString value, unsigned char salve_address, SERIAL_PORT_CONFIG config, DataAlign dataformat);
    //清洗反吹
    void ClearStart(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString cmdd);
    //2021 取设备运行状态数据
    void SBState(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    void SetSamplePeriodTime(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString CTime);
    bool GetSamplePeriodTime(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config);
    //反控
    void ControlAnalyzer(URT_PORT port, unsigned char slave_address, SERIAL_PORT_CONFIG config, QString cmdd);//传入CN指令

    void SETAWAIT(URT_PORT port, unsigned char slave_address, SERIAL_PORT_CONFIG config);

    //232Check
    static bool RS232Check(URT_PORT port, SERIAL_PORT_CONFIG config);

    //485CHeck
    static QString RS485Check(URT_PORT port, unsigned char slave_address,unsigned short reg_address,SERIAL_PORT_CONFIG config);
};

#endif // SAMPLERSTATE_H
