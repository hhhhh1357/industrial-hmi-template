#ifndef PLC_H
#define PLC_H

#include <QObject>
#include "Task.h"
#include "PLCTaskHandler2.h"
#include <QDebug>
#include <QTimer>


class PLC: public QObject
{
    Q_OBJECT

public:

    QString StrProtocol;

    PLC();
    ~PLC();
    SERIAL_PORT_CONFIG configs[7];

    PLCTaskHandler2 plcHandlers[16]={PLCTaskHandler2(URT_PORT::PORT0),PLCTaskHandler2(URT_PORT::PORT1),PLCTaskHandler2(URT_PORT::PORT2),
                                     PLCTaskHandler2(URT_PORT::PORT3),PLCTaskHandler2(URT_PORT::PORT4),PLCTaskHandler2(URT_PORT::PORT5),
                                     PLCTaskHandler2(URT_PORT::PORT6),PLCTaskHandler2(URT_PORT::PORT7),PLCTaskHandler2(URT_PORT::PORT8),
                                     PLCTaskHandler2(URT_PORT::PORT9),PLCTaskHandler2(URT_PORT::PORT10),PLCTaskHandler2(URT_PORT::PORT11),
                                     PLCTaskHandler2(URT_PORT::PORT12),PLCTaskHandler2(URT_PORT::PORT13),PLCTaskHandler2(URT_PORT::PORT14),
                                     PLCTaskHandler2(URT_PORT::PORT15)};

signals:
    void isDone(Task task);  //向 UI 发消息
    void isCloudRequestDone(Task task);   //向CloudTalker 发消息
    void isSelfReportDone(Task task);   //向Worker 发消息
    void isRequestPLC(Task task);  //向PLCTaskHandler处理的槽函数发送信号并传递数据task

public slots:
    bool requestHandler(Task task);//接收 UI 信号的槽
    void changePortConfig(SERIAL_PORT_CONFIG newValue, URT_PORT port);  //接收 UI 信号的槽。功能：修改波特率
    bool receivePlcResult(Task task);   //接收 PLC 任务处理结果
};



#endif // PLC_H
