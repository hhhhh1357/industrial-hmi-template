#ifndef PLCACTION_H
#define PLCACTION_H

typedef enum{
    GET_VALUE = 0,      //读取数值
    GET_STATUS =1,      //读取状态

    SET_INSTRUMENT_INFO=2,//设置现场机信息
    GET_INSTRUMENT_INFO=3, //上传现场机信息
    SET_Sampling_Period = 4,      //设置采样时间周期
    GET_Sampling_Period = 5,//提取/上传采样时间周期
    Sampler_Time = 6,
    GET_DEV_FLAG=7,

    CMD_ANALYZER_START=8,    //分析仪启动
    CMD_3044=9,
    CMD_3080=10,   //启动分析仪单次测试
    CMD_3085=11,
    CMD_3086=12,
    CMD_2062=13,   //提取监测指标核查数据

    CMD_SAMPLING_3015=14,   //留样
    CMD_SAMPLING_3015_CONTROL=15, //超标留样反控
    CMD_WARNING_REVERSE=16,    //告警(反向)
    CMD_WARNING_RESET=17,    //告警（还原）
    CMD_INITA = 18,      //分析仪初始化
    CMD_CHECK = 19,  //分析仪标样核查
    CMD_ANALYZER_STOP = 20,   //分析仪停止
    CMD_SAMPLER_START = 21,   //采样器启动
    CMD_SAMPLER_STOP = 22,  //采样器停止
    SET_PLC_TIME = 23  //设置PLC时间 CN=1012
}PLCAction;
#endif // PLCACTION_H
