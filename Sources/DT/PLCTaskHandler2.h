#ifndef TaskHANDLER2_H
#define TaskHANDLER2_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include "Task.h"
#include "RelayControl.h"


//PLC 任务线程
class PLCTaskHandler2: public QThread//public QObject
{
    Q_OBJECT
public:
    PLCTaskHandler2(QObject *parent=nullptr);
    PLCTaskHandler2(URT_PORT port, QObject* parent=nullptr);
    PLCTaskHandler2(const PLCTaskHandler2 &handler);

    ~PLCTaskHandler2();

    URT_PORT targetPort;
    void run() override;
    void stopRunning();
    //void run();

    Task task;

signals:

    void isDone(Task task);

private:

    bool taskResolver(Task *task);  //处理
    bool mIsStop;
    QQueue<Task> taskQueue;    //任务队列
    SERIAL_PORT_CONFIG config;   //波特率配置
    QMutex m_lock_taskQueue;
    QMutex m_lock_IsStop;

    //==============================
    Task latestTask;

    quint16 CRC16(unsigned char *data, int len);
    CMD_PACKET buildPLCCmd(PLCRequest *req);
    bool sendPLCCmd(PLCRequest *req);

    bool readSingleData(Task *task);

public slots:
    void doWork(Task task);

    void changePortConfig(SERIAL_PORT_CONFIG newValue, URT_PORT port);  //修改波特率
};


//typedef PLCTaskHandler2 * PLCTaskHandler2Ptr;

#endif // MESSAGEHANDLER_H
