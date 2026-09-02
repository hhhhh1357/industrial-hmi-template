#ifndef MFACTOR_H
#define MFACTOR_H


#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QMap>

#include "MBase.h"

// 水质采样仪类
class CWQS
{
public:
    QList<MBase *> factorList; //使用时注意强制类型转换
    QMap<QString, QString> sample_info; //水质采样仪的状态

    bool isFull =false;

    QList<QString> Factors;
    int Interval=30;
    QString PortName;
    QString PlcAddress;
    QString strProtocol;
};

class MFactor: public MBase
{
  public:
     //国标数据表对于的字段
     QString code;      //监测因子编码
     QString chName;    //监测因子中文名称
     QString oldCode;   //老国标的因子编码
     QString unitC;     //浓度单位
     QString unitE;     //排放量单位
     QString nType;     //数据类型

     //其它配置
     CWQS wqs;  //特殊配置数据。若是水质采样仪，要读取水质采样仪参数
     int     flag;      //因子类型。0：监测因子，本地存储；1：环境监测因子，不存储；2：特殊设备（水质采样器）
     bool    enabled=true;   //因子是否激活。 水质采样仪:配置了反控因子为true,否则为false;分析仪器默认为false,其它为true

     QString args;      //其它配置参数（采用JSON字符串格式保存）
     //其它配置参数字段
     QString dataPortName; //数据端口名称
     quint16 stationAddr; //从站地址
     quint16 registerAddr; //寄存器地址
     quint16 cmdCode=3; //命令码
     quint16 valueCount=2; //取值个数
     double maxValue=0;  //图表中显示的最大值 或 量程上限
     double minValue=0;  //图表中显示的最小值 或 量程下限
     double warningValueMax; //最大告警值
     double warningValueMin; //最小告警值
     QString plcDataType; //PLC数据类型（大端、小端）
     QString DataUnit; //数据单位
     QString deviceId; //检测设备编号
     SIGNAL_TYPE signalType; //模拟量的信号类型。电压：0； 电流：1
     QString localTableName; //本地数据表名

     //实际监测产生的数据
     double value;      //污染物的实际测试值
     QString dataFlag="N";  //数据标记
     int warningCount=0;  //告警记次
     QDateTime valueTimestamp; //数据时间戳

     //以下配置参数便于统计生成分钟数据
     QDateTime mStartTime;
     double mValueMax=0;
     double mValueMin=0;
     double mValueCount=0;
     double mValueSum=0;
     double mValueAvg=0;
     double mValueCou=0;
     double mValueWarningCount=0;

     QJsonObject argsToJson() const
     {
         return {
                 {"code",code},
                 {"flag",flag},
                 {"dataPortName",dataPortName},
                 {"stationAddr",stationAddr},
                 {"stationAddr",stationAddr},
                 {"registerAddr",registerAddr},
                 {"cmdCode",cmdCode},
                 {"valueCount",valueCount},
                 {"maxValue",maxValue},
                 {"minValue",minValue},
                 {"warningValueMax",warningValueMax},
                 {"warningValueMin",warningValueMin},
                 {"plcDataType",plcDataType},
                 {"deviceId",deviceId},
                 {"signalType",signalType},
                 {"localTableName",localTableName}
              };
     }

};


#endif // MFACTOR_H
