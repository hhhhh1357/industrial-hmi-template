#include "mainwindow.h"
#include "formlogin.h"
#include "framerebootcover.h"

#include <QApplication>
#include <QStyleFactory>
#include <qdesktopwidget.h>

#include "Sources/DT/Task.h"
#include "Sources/DT/plc.h"
#include "Sources/DT/PLCTaskHandler2.h"

#include <Headers/BLL/BHistoryData.h>
#include <Headers/BLL/BLocalService.h>

#include <Sources/DT/CloudServices.h>
#include <Sources/DT/ConnectStatus.h>

#include <Headers/Tools/CScreenSaver.h>

#include <Sources/Tools/sysutil.h>

//#include <QQmlEngine>

QApplication * app;

BComArgs bca;

MainWindow * mainWindow;
FormLogin * loginWindow;
PLC plc;
CloudServices *cs;
BLocalService * localService;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    app = &a;

    //定义屏保
    CScreenSaver screenSaver;
    //全局接收并委托处理事件
    a.installEventFilter(&screenSaver);

    //注册信号的参数
    qRegisterMetaType<Task>("Task");
    qRegisterMetaType<PLCRequest>("Request");
    qRegisterMetaType<PLCResponse>("Response");
    qRegisterMetaType<DataPacketData>("DataPacketData");
    qRegisterMetaType<SubWorkerBase>("SubWorkerBase");
    qRegisterMetaType<ConnectStatus>("ConnectStatus");
    qRegisterMetaType<SERIAL_PORT_CONFIG>("SERIAL_PORT_CONFIG");

    //获得本地端口信息
    bca.queryDataComArgsList();

    //启动本地服务
    BLocalService lService("LOCAL");
    localService = &lService;

    //设置本地网口/4G
    SysUtil::SetNetWork(localService->sysConfig);

    //初始化历史数据计算时间戳
    BHistoryData::initCalcTimestamp();

    //根据服务器的配置，启动云端通信服务
    QList<QString> ipAndPorts;
    for (int i=0;i<localService->sysConfig.ServerList.count();i++)
    {
        if (localService->sysConfig.ServerList.at(i)->status!=0)
        {
            QString ipAndPort = localService->sysConfig.ServerList.at(i)->ip+":"+QString::number(localService->sysConfig.ServerList.at(i)->port);
            ipAndPort += ":"+localService->sysConfig.SysDic.getMN()+":"+localService->sysConfig.ServerList.at(i)->password;
            ipAndPorts.append(ipAndPort);
        }
    }
    CloudServices cs2(ipAndPorts);
    cs = &cs2;
    localService->connectToCloudService(cs);

    //主窗口
    MainWindow w;
    mainWindow = & w;

    //请求同步时间
    localService->requestSynchronizeDatetime();
    QThread::usleep(50000);
    //请求上传数采仪开机时间
    localService->requestStartRunningTime();

    //启动本地服务的所有定时器
    localService->startTimers();

    FormLogin formLogin;
    loginWindow = &formLogin;

    //设置屏保为登录窗口
    screenSaver.setScreenFrame(loginWindow);
    screenSaver.setScreenTimeInterval(localService->sysConfig.SysDic.getSCREEN_SAVE_TIME());

    formLogin.mainWindow = &w;

    //显示COVER
    FrameRebootCover frameCover(&formLogin,nullptr);
    frameCover.show();

    //QThread::sleep(2);

    //formLogin.show();

    return a.exec();
}
