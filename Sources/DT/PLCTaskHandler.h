#ifndef TaskHANDLER_H
#define TaskHANDLER_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include "Task.h"
extern "C"{
#include "uart.h"
};
//PLC 任务线程
class PLCTaskHandler : public QThread
{
    Q_OBJECT
public:
    PLCTaskHandler();
    PLCTaskHandler(SERIAL_PORT_CONFIG config[4]);
    ~PLCTaskHandler();

    void run() override;

signals:
    void CloudTaskDone(Task );  //向Cloud发消息
    void SelfTaskDone(Task );  //向Self发消息
    void UITaskDone(Task);     //向UI发消息
private:
    //


    bool taskResolver(Task *task);  //处理
    bool mIsStop;
    QQueue<Task> taskQueue;    //任务队列
    SERIAL_PORT_CONFIG configs[4];   //波特率配置
    QMutex mqTaskQueue;
    QMutex mqIsStop;

    quint16 CRC16(unsigned char *data, int len);
    CMD_PACKET buildPLCCmd(PLCRequest *req);
    bool sendPLCCmd(PLCRequest *req);

public slots:
    bool requestHandler(Task task);//入列
    void changePortConfig(SERIAL_PORT_CONFIG newValue, URT_PORT port);  //修改波特率
};

#endif // MESSAGEHANDLER_H
