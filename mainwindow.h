#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFrame>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>
#include <QThread>

#include "Headers/BLL/BComArgs.h"
#include "Headers/BLL/BDataItem.h"
#include "Sources/DT/uart.h"
#include "Sources/DT/Task.h"
#include "Headers/BLL/BSysDictionary.h"

class CTask
{
public:
   MFactor * factor;
   Task task;
   int interval;
   QDateTime lastTime;

   CTask &operator=(const CTask &task);
};

class PLCSender:public QThread
{
    Q_OBJECT
private:
    QMutex mutexPlcTaskList;
    QList<CTask*> localPlcTaskList;
    QList<CTask*> *PLCTaskList;
    int isRunning=false;

public:
    QMutex mutexIsRunning;
    bool isFinished = false;

    PLCSender(QList<CTask*> * PLCTaskList);
    ~PLCSender();
    void run() override;
    void stopSender();
    void continueSender();
    void finishedSender();
public slots:
    void updatePlcTaskList();
signals:
    bool requestPLC(Task task); //向 PLC 发请求
};

class PLCReceiver:public QObject
{
    Q_OBJECT
private:
    MFactor * w00000=nullptr;  //污水流量因子
    MFactor * wqs=nullptr;   //水质采样仪
    QList<QList<MFactor *>*> factorList;
public:
    PLCReceiver(QList<QList<MFactor *>*> factorList, MFactor * w00000, MFactor * wqs);
    ~PLCReceiver();
    //void run() override;
    //bool isRunning=true;

    MFactor * findFactorInFactorList(Task task, bool &isFound); //查找task中的因子是否的因子表中

public slots:
    void doWork(Task task);
signals:
    void updateFactorDataItem(); //向实时数据界面发出更新实时监测数据信号
    bool requestPLC(Task task); //向 PLC 发请求
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void reboot();

    QMutex mutexDataComArgsList; //数据通讯端口列表的互斥锁
    QMutex mutexDataItemList; //实时监测因子列表的互斥锁
    QMutex mutexEnvironmentItemList; //实时监测工作环境因子列表的互斥锁

    void stopReadDataFromPLC();  //停止从PLC读取数据
    void startReadDataFromPLC(); //开始从PLC读取数据

    void stopRefreshDataCom();   //停止刷新端口状态
    void startRefreshDataCom();  //开始刷新端口状态

    void reInitialize(); // 当进行了参数配置时候，需要重新进行初始化

    QList<MFactor *> getDataItemList();        //获得实时监测因子列表
    QList<MFactor *> getEnvironmentItemList(); //获得实时监测工作环境因子列表

    QList< QList<MFactor *> * > factorList;  //所有监测因子的列表
    MFactor * wqs=nullptr;  //水质采样仪 因子指针

    PLCSender *plcSender;  //向PLC发送命令的线程对象

    PLCReceiver *plcReceiver; //接收数据对象
    QThread * plcReceiverThread; //接收线程体

signals:
    void updateFactorDataItem(); //向实时数据界面发出更新实时监测数据信号

    //发往PLC类的信号
    void changePortConfig(SERIAL_PORT_CONFIG newValue, URT_PORT port);  //修改波特率
    bool requestPLC(Task task); //向 PLC 发请求

    //将接收到的数据转发给 Receiver 线程处理
    void toReceiver(Task task);

    //向 Sender 发信号，更新命令列表
    void updatePlcTaskList();

public slots:
    bool receivePLC(Task task);  //接收 PLC 回传数据的槽   

private:
    Ui::MainWindow *ui;

    QTimer * datetimeTimer=nullptr;// 显示时间的定时器对象
    QTimer * refreshDataComTimer=nullptr; //刷新界面数据端口的定时器
  //  QTimer * readDataTimer=nullptr; //从端口读取数据定时器
  //  int readDataTimerInterval;  //从端口读取数据的定时器时间间隔（秒）

    int MenuItemSelected =1;  //主菜单项当前选择项
    QFrame * activeFrame=nullptr;  //当前显示的活动窗口

    QMap< QLabel *,MComArgs * > * DataComArgsList=nullptr; //数据通讯端口列表    

    QList<MFactor *> DataItemList; //实时监测数据列表
    QList<MFactor *> EnvironmentItemList; //实时监测工作环境项目

    MFactor * w00000=nullptr;  //污水流量因子

    QMutex mutexPLCTaskList; //向PLC发送请求数据的 任务列表的信号量
    QList<CTask*> PLCTaskList;  //向PLC发送请求数据的 任务列表

    bool eventFilter(QObject *obj, QEvent*event);  //事件拦截器
    void setComponent();  //初始化主界面相关组件

    void menuItemOnSelected(); //主菜单的菜单项选中方法

    void initDataComArgsList(); //初始化界面中数据端口状态

    void refreshDataComArgsList(); //刷新界面中数据端口状态
 //   void readData(); //从通讯端口读取数据
    void makePlcTaskList(); //生成向PLC发送请求数据的 任务列表
    void makeTask(MFactor *factor, CTask & task); //生成 task

    void setDatetimeTimer();  //设置显示时间的定时器
  //  void setRefreshDataComTimer(); //设置刷新界面数据端口状态的定时器
   // void setReadDataTimer(); //设置读取端口数据的定时器

    /*********************************************************强制重启标志************************************/
public:
    bool isRboot=false;

   // void resetGrid(QWidget *widget,double factorx,double factory);//设备分辨率


};
#endif // MAINWINDOW_H
