#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "framehistorydata.h"
#include "framerealtimedata.h"
#include "framesetup.h"
#include "framesysinfodata.h"

#include "Sources/DT/plc.h"

#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QProcess>

#include <Headers/BLL/BFactor.h>
#include <Headers/BLL/BHistoryData.h>
#include <Headers/BLL/BLocalService.h>
#include <Headers/BLL/BStatisticalData.h>

#include <Sources/Tools/datatype.h>
#include <Sources/Tools/sysutil.h>

#include <QDesktopWidget>//获取设备分辨率库

extern QApplication * app;
extern PLC plc;
extern BLocalService *localService;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    /***********************************************获取设备分辨率**********************************************/
//    //获取设备分辨率
//    int currentScreenWid = QApplication::desktop()->width();
//    int currentScreenHei = QApplication::desktop()->height();
//    //计算比例
//    double factorx=currentScreenWid/1920.0;
//    double factory=currentScreenHei/1080.0;
//    //循环遍历各控件
//    QList<QWidget *> widget = this->findChildren<QWidget *>();
//    foreach (QWidget * w, widget)
//    {
//            //为每个控件设置新的宽和高（由比例进行计算）
//            resetGrid(w,factorx,factory);
//    }
//    /*********************************************************************************************************/

    setComponent();
    menuItemOnSelected();

    initDataComArgsList();  //从数据库中读取通信端口列表参数配置

    setDatetimeTimer(); //设置显示时间的定时器

    //首次显示。显示实时数据 Frame
    this->MenuItemSelected = 1;
    activeFrame = new FrameRealTimeData(&(this->DataItemList),&(this->EnvironmentItemList), this->wqs, ui->frameMain);
    activeFrame->setGeometry(QRect(0, 0, 1280, 690));
    activeFrame->show();

    this->plcSender = new PLCSender(&(this->PLCTaskList));

    // 将 发往 PLC 的命令列表更新信号 与 Sender的槽函数连接
    connect(this,SIGNAL(updatePlcTaskList()),this->plcSender,SLOT(updatePlcTaskList()));

    // Sender 与 PLC 类建立 信号 与 槽的连接
    connect(this->plcSender, SIGNAL(requestPLC(Task)), &plc, SLOT(requestHandler(Task)));


    // PLC 回传数据信号与 Receiver 槽函数连接
    connect(&plc, SIGNAL(isDone(Task)), this, SLOT(receivePLC(Task)));
    //新添加
    //connect(this, SIGNAL(changePortConfig(SERIAL_PORT_CONFIG, URT_PORT)), &plc, SLOT(changePortConfig(SERIAL_PORT_CONFIG, URT_PORT)));

    //启动接收线程
    plcReceiverThread = new QThread();
    plcReceiver = new PLCReceiver(this->factorList, this->w00000, this->wqs);
    plcReceiver->moveToThread(plcReceiverThread);

    connect(this, SIGNAL(toReceiver(Task)), plcReceiver, SLOT(doWork(Task)));

    connect(plcReceiverThread, &QThread::finished, plcReceiver, &QObject::deleteLater);
    //将主界面的 实时数据更新 信号 与实时数据显示界面的 实时数据更新槽连接起来
    connect(plcReceiver, SIGNAL(updateFactorDataItem( )), activeFrame, SLOT(updateFactorDataItem()));
    //连接发往 PLC 命令 的信号 和 槽
    connect(plcReceiver, SIGNAL(requestPLC(Task)), &plc, SLOT(requestHandler(Task)));

    connect(&plc, SIGNAL(isDone(Task)), plcReceiver, SLOT(doWork(Task)));

    plcReceiverThread->start();

    //启动采集数据
    this->plcSender->start();
}

MainWindow::~MainWindow()
{
    //停止发往 PLC 的命令
    this->plcSender->finishedSender();
    QThread::msleep(50);

    //接收线程退出
    plcReceiverThread->quit();
    plcReceiverThread->wait();

    delete ui;

    //释放当前活动窗口
    if (activeFrame!=nullptr)
        delete activeFrame;

    delete datetimeTimer;

    {
        QMutexLocker locker1(&(BComArgs::mutexComArgsList));
        QMutexLocker locker2(&(this->mutexDataComArgsList));
        QMutexLocker locker3(&(this->mutexDataItemList));

        //释放通讯端口参数列表
        for(QMap<QLabel*,MComArgs *>::const_iterator i=DataComArgsList->constBegin();i!=DataComArgsList->constEnd();i++)
        {
            delete i.key();
        }
        delete DataComArgsList;

        //释放通信端口列表
        for (int i=0;i<BComArgs::ComArgsList.count();i++)
            delete BComArgs::ComArgsList.at(i);

    }
    {
        //释放发往 PLC 的命令任务列表
        QMutexLocker locker4(&(this->mutexPLCTaskList));
        for (int i=0;i<this->PLCTaskList.count();i++)
        {
            delete this->PLCTaskList.at(i);
        }
        this->PLCTaskList.clear();
    }

    //delete plcSender;
}

//重启系统
void MainWindow::reboot()
{
    //this->hide();

    this->plcSender->finishedSender();
    QThread::msleep(100);

    this->close();

    QString program = QApplication::applicationFilePath();
    QStringList args = QApplication::arguments();
    QString workDir = QDir::currentPath();
    QProcess::startDetached(program,args,workDir);
    QApplication::exit();
}

void MainWindow::stopReadDataFromPLC()
{
    this->plcSender->stopSender();
}

void MainWindow::startReadDataFromPLC()
{
    this->plcSender->continueSender();
}

void MainWindow::stopRefreshDataCom()
{
    if (this->refreshDataComTimer!=nullptr && this->refreshDataComTimer->isActive()==true)
    {
        this->refreshDataComTimer->stop();
    }
}

void MainWindow::startRefreshDataCom()
{
    if (this->refreshDataComTimer!=nullptr && this->refreshDataComTimer->isActive()==false)
    {
        this->refreshDataComTimer->start();
    }
}

void MainWindow::reInitialize()
{
    {
        QMutexLocker locker1(&(BComArgs::mutexComArgsList));
        QMutexLocker locker2(&(this->mutexDataComArgsList));
        QMutexLocker locker3(&(this->mutexDataItemList));

        //(1)释放原有的空间
        //释放通讯端口参数列表

        QList<QLabel*> keys = DataComArgsList->keys();

        for (int i=0;i<keys.count();i++)
        {
            delete  keys.at(i);
        }

        DataComArgsList->clear();
        delete DataComArgsList;
        DataComArgsList = nullptr;

        this->wqs = nullptr;

        //清除实时数据列表
        DataItemList.clear();
        EnvironmentItemList.clear();
        factorList.clear();

        BComArgs bca;
        bca.queryDataComArgsList();

    }
    initDataComArgsList();

    //让 Sender 更新发往 PLC 的命令列表
    emit updatePlcTaskList();
}

QList<MFactor *> MainWindow::getDataItemList()
{
    return this->DataItemList;
}

QList<MFactor *> MainWindow::getEnvironmentItemList()
{
    return this->EnvironmentItemList;
}


//接收 PLC 回传数据的 槽函数
bool MainWindow::receivePLC(Task task)
{
    //qDebug()<<__FILE__<<" : "<<__FUNCTION__<<": task:"<<task.req.factorCode<<","<<task.res.result<<endl;
    emit toReceiver(task);
}


void MainWindow::setComponent()
{
    //为按钮设置事件过滤器
    ui->lbShutdown->installEventFilter(this);

    ui->lbSysInfo->installEventFilter(this);
    ui->lbSetup->installEventFilter(this);
    ui->lbHistoryData->installEventFilter(this);
    ui->lbRealtimeData->installEventFilter(this);
}

//设置显示时间的定时器
void MainWindow::setDatetimeTimer()
{
    //datetimeTimer = new QTimer(this->centralWidget());
    datetimeTimer = new QTimer();
    connect(datetimeTimer,&QTimer::timeout,[=](){

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString cdt =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

       // qDebug()<<__FILE__<<" : datetimeTimer : "<<cdt<<endl;

        ui->lbTime->setText(cdt);

        if (current_date_time.time().second() % 2 ==1)  //刷新端口状态
           refreshDataComArgsList();

        });

    datetimeTimer->setInterval(500);  //设置定时器启动的时间间隔为 0.5 秒
    if (datetimeTimer->isActive() == false)
    {
           datetimeTimer->start();
    }
}
//*******************************************************获取设备分辨率****************************************/
//void MainWindow::resetGrid(QWidget *widget, double factorx, double factory)
//{
//    //获取每个窗口控件原有的位置和宽高
//    int widgetX = widget->x();
//    int widgetY = widget->y();
//    int widgetWid = widget->width();
//    int widgetHei = widget->height();
//    //计算改变分辨率后每个窗口控件的位置和宽高
//    int nWidgetX = (int)(widgetX*factorx);
//    int nWidgetY = (int)(widgetY*factory);
//    int nWidgetWid = (int)(widgetWid*factorx);
//    int nWidgetHei = (int)(widgetHei*factory);
//    //为每个窗口控件设置新的宽高
//    widget->setGeometry(nWidgetX,nWidgetY,nWidgetWid,nWidgetHei);
//}

//主菜单项事件过滤器
bool MainWindow::eventFilter(QObject *obj, QEvent*event)
{
    if (event->type()==QEvent::MouseButtonPress)
    {
        if (isRboot==true)  //强制重启
        {
            QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", "重要配置参数修改了，系统将重新启动！！！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            reboot();
            return true;
        }


        if (obj==ui->lbShutdown)  //重启按钮
        {          
               if (QMessageBox::Yes==QMessageBox::question(nullptr,tr("提示"),tr("确定要重新启动设备吗？"),
                                                           QMessageBox::Yes|QMessageBox::No, QMessageBox::No))
               {
                   reboot();
               }
               return true;
        }
        else if ((obj==ui->lbSysInfo&& MenuItemSelected!=4) ||
                 (obj==ui->lbSetup&& MenuItemSelected!=3) ||
                 (obj==ui->lbHistoryData && MenuItemSelected!=2)||
                 (obj==ui->lbRealtimeData && MenuItemSelected!=1))
        {
            //根据之前的界面状态，拆除信号和槽的连接
            if (activeFrame!=nullptr && MenuItemSelected == 1) //之前的界面是实时数据显示界面
            {
                disconnect(this, SIGNAL(updateFactorDataItem()), 0, 0);
            }

            // 清除 frameMain 上的组件
            QList<QWidget*> items = ui->frameMain->findChildren<QWidget*>();
            if (items.count()>0) delete items[0];

            // 判断哪个菜单选中
            if (obj==ui->lbSysInfo && MenuItemSelected!=4) //系统信息菜单
            {
                this->MenuItemSelected = 4;
                activeFrame = new FrameSysInfoData(ui->frameMain);
                activeFrame->setGeometry(QRect(0,0,1280,690));
                activeFrame->show();
            }
            else if (obj==ui->lbSetup && MenuItemSelected!=3) //系统设置菜单
            {
                this->MenuItemSelected = 3;
                activeFrame = new FrameSetup(ui->frameMain);
                activeFrame->setGeometry(QRect(0, 0, 1280, 690));
                activeFrame->show();
            }
            else if (obj==ui->lbHistoryData && MenuItemSelected!=2) //历史数据菜单
            {
                this->MenuItemSelected = 2;
                activeFrame = new FrameHistoryData(ui->frameMain);
                activeFrame->setGeometry(QRect(0, 0, 1280, 690));
                activeFrame->show();
            }
            else if (obj==ui->lbRealtimeData && MenuItemSelected!=1) //实时数据菜单
            {
                this->MenuItemSelected = 1;
                activeFrame = new FrameRealTimeData(&(this->DataItemList),&(this->EnvironmentItemList),this->wqs, ui->frameMain);
                activeFrame->setGeometry(QRect(0, 0, 1280, 690));
                activeFrame->show();

                //将主界面的 实时数据更新 信号 与实时数据显示界面的 实时数据更新槽连接起来
                connect(plcReceiver, SIGNAL(updateFactorDataItem( )), activeFrame, SLOT(updateFactorDataItem()));
            }
            menuItemOnSelected();
        }
        return true;
    }
    else
    {
        return QWidget::eventFilter(obj,event);
    }
}

//主菜单项选择响应方法
void MainWindow::menuItemOnSelected()
{

    QString style="background:radial-gradient(circle, #10215C 0%, #070F29 100%);background-color: transparent;color:white;";

    QString mstyle = style + "image:url(:/Images/datasys.png)";
    ui->lbRealtimeData->setStyleSheet(mstyle);
    mstyle = style + "image:url(:/Images/datahis.png)";
    ui->lbHistoryData->setStyleSheet(mstyle);
    mstyle = style + "image:url(:/Images/SysD.png)";
    ui->lbSetup->setStyleSheet(mstyle);
    mstyle = style + "image:url(:/Images/about.png)";
    ui->lbSysInfo->setStyleSheet(mstyle);
    mstyle = style + "image:url(:/Images/switch0.png)";
    ui->lbShutdown->setStyleSheet(mstyle);

    // 选中的菜单项设置指定背景色
    switch(this->MenuItemSelected)
    {
        case 1:
           mstyle = style + "image:url(:/Images/SelDatasys.png)";
           ui->lbRealtimeData->setStyleSheet(mstyle);
           break;
        case 2:
           mstyle = style + "image:url(:/Images/SelDatahis.png)";
           ui->lbHistoryData->setStyleSheet(mstyle);
           break;
        case 3:
           mstyle = style + "image:url(:/Images/SelSysD.png)";
           ui->lbSetup->setStyleSheet(mstyle);
           break;
        case 4:
           mstyle = style + "image:url(:/Images/SelAbout.png)";
           ui->lbSysInfo->setStyleSheet(mstyle);
           break;
        case 5:
           mstyle = style + "image:url(:/Images/switch0.png)";
           ui->lbShutdown->setStyleSheet(mstyle);
           break;
    }
}

//初始化数据通讯端口列表
void MainWindow::initDataComArgsList()
{
    QMutexLocker locker1(&(BComArgs::mutexComArgsList));
    QMutexLocker locker2(&(this->mutexDataComArgsList));
    QMutexLocker locker3(&(this->mutexDataItemList));

    DataComArgsList = new QMap<QLabel *,MComArgs *>();

    QFont ft;
    ft.setPointSize(8);

    for (int i=0;i<BComArgs::ComArgsList.count();i++)
    {
         MComArgs * mca = BComArgs::ComArgsList.at(i);

         //加底部状态显示的图标
         QLabel * label = new QLabel();
         QString style = "color:white;background-color: transparent;background-image:url(':/"+BComArgs::getComIconFile(mca)+"');";
         label->setStyleSheet(style);
         label->setFixedSize(32,32);
         label->setContentsMargins(0,22,0,0);
         label->setText(mca->PortName.right(1));
         label->setFont(ft);
         DataComArgsList->insert(label, mca );
         ui->hLayoutDataPort->addWidget(label);

         if(mca->Status!=UNUSED)
         {
             //将监测因子加入到实时数据表
             for (int j=0;j<mca->ComFactorArgsList.count();j++)
             {
                 if (((MFactor *)(&(mca->ComFactorArgsList.at(j))))->flag==0)  //普通监测因子
                     DataItemList.append((MFactor *)(&(mca->ComFactorArgsList.at(j))));
                 else if (((MFactor *)(&(mca->ComFactorArgsList.at(j))))->flag==1) //环境监测因子
                     EnvironmentItemList.append((MFactor *)(&(mca->ComFactorArgsList.at(j))));
                 else if (((MFactor *)(&(mca->ComFactorArgsList.at(j))))->flag==2) //水质采样仪
                     wqs = ((MFactor *)(&(mca->ComFactorArgsList.at(j))));
             }
         }
         // 发送端口初始化信号
         // 初始化 使用的 RS232 和 RS485
         if ( mca->Status != UNUSED && (mca->PortType == "RS232" || mca->PortType == "RS485"))
         {
             SERIAL_PORT_CONFIG config;
             URT_PORT port=(URT_PORT) (mca->CId - 1) ; //端口号
             config.baud = (BAUD_RATE) mca->Speed;
             config.dataBits = (DATA_BITS) mca->DataBit;
             config.stopBits = (STOP_BITS) mca->StopBit;
             config.checkBits = (PARITY_CHECK) mca->CheckBit;
             config.startBits = START_ONE; //起始位
             //emit changePortConfig(config,port);
             plc.changePortConfig(config,port);
         }
    }

    if (this->wqs!=nullptr)
    {
        //初始化水质采样仪控制因子的列表
        for (int i=0;i<DataItemList.count();i++)
        {
            for (int j=0;j<wqs->wqs.Factors.count();j++)
            {
                if (((MFactor *)DataItemList.at(i))->code == wqs->wqs.Factors.at(j))
                {
                    wqs->wqs.factorList.append((MFactor *)DataItemList.at(i));
                    ((MFactor *)DataItemList.at(i))->enabled = false; //分析仪初始不采样
                    break;
                }
            }
        }
        wqs->enabled = true;
//        if (wqs->wqs.factorList.count()>0)
//            wqs->enabled = true;
    }
    //生成向PLC发命令的列表
    makePlcTaskList();

    factorList.append(&DataItemList);
    factorList.append(&EnvironmentItemList);

}

//刷新界面中数据端口状态
void MainWindow::refreshDataComArgsList()
{

//qDebug()<<__FILE__<<", "<< __FUNCTION__<<endl;
//return;

    QDateTime dt = QDateTime::currentDateTime();
    QMutexLocker locker1(&(BComArgs::mutexComArgsList));
    QMutexLocker locker2(&(this->mutexDataComArgsList));

    if (DataComArgsList!=nullptr)
    {
        for(QMap<QLabel*,MComArgs *>::const_iterator i=DataComArgsList->constBegin();i!=DataComArgsList->constEnd();i++)
        {
            MComArgs *mca = (MComArgs *)(i.value());
            if (mca->Status!=0 && mca->PortType=="SERVER")  //服务器状态
            {
                for (int i=0;i<2;i++)
                {
                    if (QString::number(i+1)==mca->PortName.right(1))
                    {
                        if (localService->sysConfig.ServerList.at(i)->status == 1)
                            mca->Status = NORMAL;
                        else
                            mca->Status = ERROR;
                        mca->isUpdate = true;
                    }
                }
            }
            else if (mca->Status!=0 && mca->PortType=="4G-DTU")   //4G-DTU 的状态
            {
                if (dt.time().second()==20 && dt.time().msec()<600)
                {
                    if (localService->sysConfig.dtu4G.status !=0 && SysUtil::get4gDtuStatus())
                        mca->Status = NORMAL;
                    else
                        mca->Status = ERROR;
                    mca->isUpdate = true;
                }
            }
            else if (mca->Status!=0 && mca->PortType=="NETPORT")  //以太网络状态
            {
                if (dt.time().second()==15 && dt.time().msec()<600)
                {
                    for (int i=0;i<2;i++)
                    {
                        if (QString::number(i+1)==mca->PortName.right(1))
                        {
                            if (localService->sysConfig.EthernetList.at(i)->status !=0 && SysUtil::getEthernetStatus(mca->PortName))
                                mca->Status = NORMAL;
                            else
                                mca->Status = ERROR;
                            mca->isUpdate = true;
                        }
                    }
                }
            }
            else if (mca->Status!=0 && mca->PortType=="S") //开关量状态
            {
                if (dt.time().second()==30 && dt.time().msec()<600)
                {
                    if(SysUtil::getSwitchStatus(mca->PortName))
                        mca->Status = NORMAL;
                    else
                        mca->Status = ERROR;
                    mca->isUpdate = true;
                }
            }
            else if (mca->Status!=0 && mca->PortType=="R")//继电器状态
            {
                if(SysUtil::getRelayStatus(mca->PortName))
                    mca->Status = NORMAL;
                else
                    mca->Status = ERROR;
                mca->isUpdate = true;


            }

            if (mca->isUpdate)
            {
               QLabel * label = i.key();
               QString style = "color:white; background-color: transparent ;background-image:url(':/"+BComArgs::getComIconFile(mca)+"');";
               label->setStyleSheet(style);
               mca->isUpdate = false;
            }
        }
    }
}

void MainWindow::makePlcTaskList()
{
    QMutexLocker lock(&(this->mutexPLCTaskList));
    this->PLCTaskList.clear();

    //轮询所有的监测因子
    QList<MFactor *> * list;
    for (int i=0;i<2;i++)
    {
        if (i==0)
            list = & (this->DataItemList);
        else if (i==1)
            list = & (this->EnvironmentItemList);

        for (int i=0;i<list->count();i++)
        {
            MFactor * fact = list->at(i);

            //记录污水流量因子，便于计算累积量
            if (fact->code=="w00000")
                w00000 = fact;

            CTask *task = new CTask();
            makeTask(fact, (*task));
            this->PLCTaskList.append(task);
        }
    }

    if (this->wqs!=nullptr)
    {
        //水质分析仪
        CTask *task = new CTask();
        makeTask(this->wqs, (*task));
        this->PLCTaskList.append(task);
    }
    //轮询开关量
//    for (int i=0;i<BComArgs::ComArgsList.count();i++)
//    {

//    }

}

void MainWindow::makeTask(MFactor * fact, CTask &task)
{
    SysDictionary sdic;
    task.factor = fact;
    task.task.req.factorCode = fact->code; //监测因子编号
    if (fact->code.left(1)=="e"||fact->code.left(1)=="g")
    {
        task.task.req.factorCode = task.task.req.factorCode.replace("xx",fact->deviceId);
    }

    task.task.req.repeats = sdic.getREPEAT();

    if (fact->dataPortName.left(2)=="RS") //数字量端口
    {
        task.task.req.cmdCode = (unsigned char)(fact->cmdCode);//+10000).right(2);
        task.task.req.valueCount = fact->valueCount;
        //qDebug()<<"-------------------->>>"<<task.task.req.valueCount<<endl;
        task.task.req.stationNo = (unsigned char) (fact->stationAddr);
        task.task.req.plcAddr = fact->registerAddr;
        if (fact->plcDataType=="H1234")
           task.task.req.dataAlign = (DataAlign)1;
        else if (fact->plcDataType=="H4321")
            task.task.req.dataAlign = (DataAlign)0;
        else if (fact->plcDataType=="H3412")
            task.task.req.dataAlign = (DataAlign)2;
        else
            task.task.req.dataAlign = (DataAlign)2;

        // 注意端口映射的计算
        task.task.req.port =(URT_PORT)((fact->dataPortName.left(5)=="RS232")?fact->dataPortName.right(1).toInt()-1:fact->dataPortName.right(1).toInt()+4);
    }
    else if (fact->dataPortName.left(2)=="AI")  //模拟量
    {
        task.task.req.port = (URT_PORT)(fact->dataPortName.right(1).toInt() + 6); //模拟量通道端口编号：7-14
    }

    if (fact->code == "wqs")  //水质采样仪
    {
        printf("fact->code: %s\n", fact->code);
        task.task.req.action = PLCAction::GET_STATUS;
        printf("fact->wqs.strProtocol: %s\n", fact->wqs.strProtocol);
        plc.StrProtocol = fact->wqs.strProtocol;
        printf("plc.StrProtocol: %s\n", plc.StrProtocol);
        task.interval = fact->wqs.Interval;
    }
    else
    {
       task.task.req.action = PLCAction::GET_VALUE;
       task.interval = sdic.getINTERVAL();
    }
}

//发送命令的线程类
PLCSender::PLCSender(QList<CTask*> *PLCTaskList)
{
    this->PLCTaskList = PLCTaskList;

    for (int i=0;i<this->PLCTaskList->count();i++)
    {
        CTask * task = new CTask();
        *task = *(PLCTaskList->at(i));
        this->localPlcTaskList.append(task);
    }

    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
}

PLCSender::~PLCSender()
{
    QMutexLocker lock(&(this->mutexPlcTaskList));
    for (int i=0;i<localPlcTaskList.count();i++)
    {
        delete  localPlcTaskList.at(i);
    }
}

void PLCSender::run()
{
    //等到整5秒数启动
    QDateTime lastTime = QDateTime::currentDateTime();
    while(lastTime.time().second()%5!=0)
    {
       msleep(50);
       lastTime = QDateTime::currentDateTime();
    }
    this->isRunning = true;

    int cnt = this->localPlcTaskList.count();
    for (int i=0;i<cnt;i++)
    {
        CTask *task = this->localPlcTaskList.at(i);
        task->lastTime = lastTime;
    }

    while(1)
    {
        //判断是否要暂停
        do {
            bool r=false;
            {
                QMutexLocker locker1(&(this->mutexIsRunning));
                r=this->isRunning;
            }
            if (r==false)
                usleep(1000);
            else
                break;
        }while(1);

        {  // 判断线程是否人为结束
           QMutexLocker locker1(&(this->mutexIsRunning));
           if (this->isFinished)
               break;
        }

        msleep(500);

        lastTime = QDateTime::currentDateTime();

        int cnt = this->localPlcTaskList.count();
 //qDebug()<<__FILE__<<", "<<__FUNCTION__<<", localPlcTaskList's count ="<<cnt<<endl;
        for (int i=0;i<cnt;i++)
        {
            CTask *task = this->localPlcTaskList.at(i);

            if (task->factor->enabled==false)
                continue;
            if (task->lastTime.addSecs(task->interval)<=lastTime)
            {

 //qDebug()<<__FILE__<<", "<<__FUNCTION__<<", request: interval="<<task->interval<<",port="<<task->task.req.port<<endl;
                //task->lastTime = task->task.req.taskBegin = task->lastTime.addSecs(task->interval);
                task->lastTime = task->task.req.taskBegin = lastTime;
                emit requestPLC(task->task);
                //msleep(150); //间隔 50 ms
            }
            {
                QMutexLocker locker1(&(this->mutexIsRunning));
                if (this->isFinished)
                    break;
            }
        }
        {
            QMutexLocker locker1(&(this->mutexIsRunning));
            if (this->isFinished)
                break;
        }

    }
}

void PLCSender::stopSender()
{
    QMutexLocker locker1(&(this->mutexIsRunning));
    this->isRunning=false;
}

void PLCSender::continueSender()
{
    QMutexLocker locker1(&(this->mutexIsRunning));
    this->isRunning=true;
}

void PLCSender::finishedSender()
{
    QMutexLocker locker1(&(this->mutexIsRunning));
    this->isFinished=true;
}

void PLCSender::updatePlcTaskList()
{
    this->stopSender();
    msleep(10);

    while (this->isRunning)
    {
       msleep(50);
    }

    {
        QMutexLocker lock(&(this->mutexPlcTaskList));
        for (int i=0;i<localPlcTaskList.count();i++)
        {
            delete  localPlcTaskList.at(i);
        }
        this->localPlcTaskList.clear();
        for (int i=0;i<this->PLCTaskList->count();i++)
        {
            CTask * task = new CTask();
            *task = *(PLCTaskList->at(i));
            this->localPlcTaskList.append(task);
        }
    }
    this->continueSender();
}

PLCReceiver::PLCReceiver(QList<QList<MFactor *> *> factorList, MFactor *w00000, MFactor * wqs)
{
    this->factorList = factorList;
    this->wqs = wqs;
    this->w00000 = w00000;
}

PLCReceiver::~PLCReceiver()
{
    //   qDebug()<<__FILE__<<", "<<__FUNCTION__<<endl;
}

MFactor *PLCReceiver::findFactorInFactorList(Task task, bool &isFound)
{
        MFactor *fact=nullptr;
        int i=0;
        QString port="";

        for (int k=0;k<factorList.count();k++)
        {
            QList<MFactor*> * fList;
            fList = factorList.at(k);
            int i;
            for (i=0;i<fList->count();i++)
            {
                fact = fList->at(i);
                if (task.req.factorCode.left(1)=='e'||task.req.factorCode.left(1)=='g')
                {
                    if (port==fact->dataPortName && task.req.factorCode.left(4) == fact->code.left(4) && task.req.factorCode.right(2)==fact->deviceId)
                        isFound = true;
                    else
                        isFound = false;
                }
                else
                {
                    if (port==fact->dataPortName && task.req.factorCode == fact->code)
                        isFound = true;
                    else
                        isFound = false;
                }
                if (isFound)
                    break;
            }
            if (isFound)
                break;
        }

        if (isFound)
            return fact;
        else
            return nullptr;
}

void PLCReceiver::doWork(Task task)
{
   // qDebug()<<__FILE__<<" : "<<__FUNCTION__<<": task:"<<task.req.factorCode<<","<<task.res.result<<",#singleData="<<task.res.singleData.v<<endl;

    //生成端口名称
    QString port="";
    if (task.req.port<=4)
    {
       port = "RS232-" + QString::number(task.req.port+1);
    }
    else if (task.req.port>=5 && task.req.port<=6)
    {
        port = "RS485-" + QString::number(task.req.port-4);
    }
    else if (task.req.port>=7 && task.req.port<=14)
    {
        port = "AI-"+QString::number(task.req.port-6);
    }

    MComArgs curr_mca;
    curr_mca.CId=-999;

   //修改端口状态，获取端口值
    for (int i=0;i<BComArgs::ComArgsList.count();i++)
    {
        if (BComArgs::ComArgsList.at(i)->PortName == port)
        {
            curr_mca = * (BComArgs::ComArgsList.at(i));

            if (task.res.result == false )
            {
                if (BComArgs::ComArgsList.at(i)->Status != ERROR)
                {
                   BComArgs::ComArgsList.at(i)->Status = ERROR ;
                   BComArgs::ComArgsList.at(i)->isUpdate = true;
                }
                else if (BComArgs::ComArgsList.at(i)->Status == ERROR)
                {
                   BComArgs::ComArgsList.at(i)->isUpdate = false;
                }
            }
            else
            {
                if (BComArgs::ComArgsList.at(i)->Status == NORMAL)
                {
                   BComArgs::ComArgsList.at(i)->isUpdate = false;
                }
                else if (BComArgs::ComArgsList.at(i)->Status == ERROR)
                {
                   BComArgs::ComArgsList.at(i)->Status = NORMAL ;
                   BComArgs::ComArgsList.at(i)->isUpdate = true;
                }
            }
            break;
        }
    }

    qDebug()<<"=====task.req.factorCode = "<<task.req.factorCode<<endl;
    qDebug()<<"=====task.res.result = "<<task.res.result<<endl;
    if (task.res.result == false ) //读数据错误
    {
        MFactor *fact=nullptr;
        bool isFound=false;
        fact= findFactorInFactorList(task,isFound);
        if (fact!=nullptr)
        {
            fact->value=0;//123456789; //设置成一个特殊值
            emit updateFactorDataItem( );
        }

        return;
    }

    if (task.req.factorCode=="wqs")  //水质采样仪
    {
        wqs->wqs.sample_info = task.res.resultMap;

//qDebug()<<"WQS status:"<<endl;
//for(QMap<QString,QString>::ConstIterator ite=task.res.resultMap.constBegin(); ite!=task.res.resultMap.constEnd(); ++ite)
//    qDebug()<<"***"<<ite.key()<<": "<<ite.value()<<endl;

        //判断是否水满
        if (wqs->wqs.sample_info.value("WaterSupplyFlag")=="1" && wqs->wqs.isFull==false)
        //if (wqs->wqs.sample_info.value("WaterSupplyFlag")=="0")
        {
            wqs->wqs.isFull = true;
            //发启动命令
            Task task;
            for (int i=0;i<wqs->wqs.factorList.count();i++)
            {
                MFactor * fact = (MFactor *)wqs->wqs.factorList.at(i);
                SysDictionary sdic;
                task.req.factorCode = fact->code; //监测因子编号
                if (fact->code.left(1)=="e"||fact->code.left(1)=="g")
                {
                    task.req.factorCode = task.req.factorCode.replace("xx",fact->deviceId);
                }

                task.req.repeats = sdic.getREPEAT();

                if (fact->dataPortName.left(2)=="RS") //数字量端口
                {
                    task.req.cmdCode = (unsigned char)(fact->cmdCode);//+10000).right(2);
                    task.req.valueCount = fact->valueCount;
                    task.req.stationNo = (unsigned char) (fact->stationAddr);
                    task.req.plcAddr = fact->registerAddr;
                    if (fact->plcDataType=="H1234")
                       task.req.dataAlign = (DataAlign)1;
                    else if (fact->plcDataType=="H4321")
                       task.req.dataAlign = (DataAlign)0;
                    else if (fact->plcDataType=="H3412")
                       task.req.dataAlign = (DataAlign)2;
                    else
                       task.req.dataAlign = (DataAlign)2;

                    // 注意端口映射的计算
                    task.req.port =(URT_PORT)((fact->dataPortName.left(5)=="RS232")?fact->dataPortName.right(1).toInt()-1:fact->dataPortName.right(1).toInt()+4);
                }
                else if (fact->dataPortName.left(2)=="AI")  //模拟量
                {
                    task.req.port = (URT_PORT)(fact->dataPortName.right(1).toInt() + 6); //模拟量通道端口编号：7-14
                }
                task.req.action = PLCAction::CMD_ANALYZER_START;

                emit requestPLC(task);
            }

            for (int i=0;i<wqs->wqs.factorList.count();i++)
            {
               ((MFactor *) (wqs->wqs.factorList.at(i)))->enabled = true;
            }
        }
        else if (wqs->wqs.sample_info.value("WaterSupplyFlag")=="0")
        {
            wqs->wqs.isFull = false;
        }
    }
    else  //普通因子 或 环境因子
    {
        MFactor *fact=nullptr;
        bool isFound=false;
        int i=0;

        for (int k=0;k<factorList.count();k++)
        {
            QList<MFactor*> * fList;
            fList = factorList.at(k);
            int i;
            for (i=0;i<fList->count();i++)
            {
                fact = fList->at(i);
                if (task.req.factorCode.left(1)=='e'||task.req.factorCode.left(1)=='g')
                {
                    if (port==fact->dataPortName && task.req.factorCode.left(4) == fact->code.left(4) && task.req.factorCode.right(2)==fact->deviceId)
                        isFound = true;
                    else
                        isFound = false;
                }
                else
                {
                    if (port==fact->dataPortName && task.req.factorCode == fact->code)
                        isFound = true;
                    else
                        isFound = false;
                }
                if (isFound)
                    break;
            }
            if (isFound)
                break;
        }

        if (isFound)
        {
             if (fact->flag==0 || fact->flag==1)
            {
                //fact->valueTimestamp = QDateTime::currentDateTime();
                fact->valueTimestamp = task.req.taskBegin;

 //模拟调试代码
// {
//     fact->plcDataType = "H1234";
//     if (fact->maxValue!=fact->minValue)
//         task.res.singleData.v = RandomData::getRadomData(fact->value+(fact->maxValue-fact->minValue)*0.05,
//                                                          fact->value-(fact->maxValue-fact->minValue)*0.05);
//     else
//         task.res.singleData.v = qrand()%2;

//     if (task.res.singleData.v==0) task.res.singleData.v=1;
//     else if (task.res.singleData.v==1) task.res.singleData.v=0;

//     task.res.result = true;
// }

                 if (fact->dataPortName.left(2)=="RS")  //数字量通道
                 {
                     fact->value =task.res.singleData.v;
                 }
                 else if (fact->dataPortName.left(2)=="AI")  //模拟量通道，数据要转化
                 {
                     fact->value =task.res.singleData.v;
                     if (fact->signalType != VOLTAGE)   //信号类型设置为电流时，需要进行转换
                          //fact->value /= 200;
                         fact->value *= 5;

                     //因子观测值=(因子上限值-因子下限值)/(电信号上限-电信号下限)*(电信号观测值-电信号下限)
                     //fact->value = (fact->warningValueMax-fact->warningValueMin)/(fact->maxValue-fact->minValue)*(task.res.singleData.v-fact->minValue);

                     fact->value =curr_mca.FactorDataRangeMin + (curr_mca.FactorDataRangeMax-curr_mca.FactorDataRangeMin)/(curr_mca.DataRangeMax-curr_mca.DataRangeMin)*(fact->value-curr_mca.DataRangeMin);

                 }
                 else
                 {
                     fact->value =task.res.singleData.v;
                 }
                 if(fact->code=="a05001")
                 {
                    fact->value = fact->value *44/22.4*10000;
                 }

                 //设置数据标识
                 if (fact->value<fact->warningValueMin)
                 {
                     fact->dataFlag="L";
                     fact->warningCount ++;
                 }
                 else if (fact->value>fact->warningValueMax)
                 {
                     fact->dataFlag="T";
                     fact->warningCount ++;
                 }
                 else
                 {
                     fact->warningCount=0;
                     fact->dataFlag="N";
                 }
             }

             if (fact->flag==0 && fact->warningCount>=1)  //数据超限
             {
                 Task task;
                 task.req.taskBegin = QDateTime::currentDateTime();
//qDebug()<<"Data value outoff warning value range......"<<"factor:"<<fact->code<<","<<fact->value<<endl;
                 //触发继电器动作
                 int ch1, ch2;
                 BComArgs bca;
                 bool r = bca.getUsingRelayPort(ch1,ch2);
                 if (r)
                 {
                     task.req.port = (URT_PORT)15;
                     task.req.ch1 = (RELAYER_CHANNEL) ch1;
                     task.req.ch2 = (RELAYER_CHANNEL) ch2;
                     task.req.action=PLCAction::CMD_WARNING_REVERSE;
                     emit requestPLC(task);
//qDebug()<<"Request Relay action......"<<endl;
                 }

                 //触发超标留样
                 if (this->wqs!=nullptr && this->wqs->dataPortName.left(2)=="RS") //数字量端口
                 {
                     task.req.cmdCode = (unsigned char)(wqs->cmdCode);//+10000).right(2);
                     task.req.valueCount = wqs->valueCount;
                     task.req.stationNo = (unsigned char) (wqs->stationAddr);
                     task.req.plcAddr = wqs->registerAddr;
                     if (wqs->plcDataType=="H1234")
                        task.req.dataAlign = (DataAlign)1;
                     else if (wqs->plcDataType=="H4321")
                        task.req.dataAlign = (DataAlign)0;
                     else if (wqs->plcDataType=="H3412")
                        task.req.dataAlign = (DataAlign)2;
                     else
                        task.req.dataAlign = (DataAlign)2;

                     // 注意端口映射的计算
                     task.req.port =(URT_PORT)((wqs->dataPortName.left(5)=="RS232")?wqs->dataPortName.right(1).toInt()-1:wqs->dataPortName.right(1).toInt()+4);
                 }
                 else if (fact->dataPortName.left(2)=="AI")  //模拟量
                 {
                     task.req.port = (URT_PORT)(fact->dataPortName.right(1).toInt() + 6); //模拟量通道端口编号：7-14
                 }

                 task.req.action = PLCAction::CMD_SAMPLING_3015;
//qDebug()<<"Request sampling......"<<endl;
                 emit requestPLC(task);
             }

             if (fact->flag==0) //监测因子
             {
                 //qDebug()<<"Write data to database......"<<"factor:"<<fact->code<<","<<fact->value<<endl;
                 //保存数据到本地数据库中
                 BHistoryData bHistoryData;
                 bHistoryData.writeToDB(fact);

                 static QDateTime mValueTimestamp = QDateTime::currentDateTime();
                 //计算分钟数据
                 //if (fact->mValueCount==0 && fact->valueTimestamp.time().minute()%5==0)
                 //if (fact->mValueCount==0 && mValueTimestamp.time().minute()%5==0)
                 if (fact->mValueCount==0)
                 {
                     //从整5分钟开始计算分钟数据
                     //fact->mStartTime = fact->valueTimestamp;
                     fact->mStartTime = mValueTimestamp;
                     fact->mValueMax = fact->value;
                     fact->mValueMin = fact->value;
                     fact->mValueSum = fact->value;
                     fact->mValueCount = 1;
                     if (fact->dataFlag!="N")
                         fact->mValueWarningCount = 1;
                     if (w00000!=nullptr)
                     {
                         if (fact->code.left(1)=="w" && fact->code!="w00000")
                         {
                             fact->mValueCou = w00000->value * fact->value * localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE();
                         }
                         else if (fact->code=="w00000")
                         {
                             fact->mValueCou = fact->value * localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE();
                         }
                     }
                 }
                 //else if (fact->mValueCount>0 && fact->valueTimestamp<fact->mStartTime.addSecs(localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))
                 else if (fact->mValueCount>0 && fact->valueTimestamp<fact->mStartTime.addSecs(localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))
                 {
                     fact->mValueMax = fact->mValueMax>fact->value ? fact->mValueMax : fact->value;
                     fact->mValueMin = fact->mValueMin<fact->value ? fact->mValueMin : fact->value;
                     fact->mValueSum += fact->value;
                     fact->mValueCount++;
                     if (fact->dataFlag!="N")
                         fact->mValueWarningCount ++;
                     if (w00000!=nullptr)
                     {
                         if (fact->code.left(1)=="w" && fact->code!="w00000")
                         {
                             fact->mValueCou += w00000->value * fact->value * localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE();
                         }
                         else if (fact->code=="w00000")
                         {
                             fact->mValueCou += fact->value * localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE();
                         }
                     }
                 }
                 //else if (fact->mValueCount>0 && fact->valueTimestamp>fact->mStartTime.addSecs(localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))
                 else if (fact->mValueCount>0 && fact->valueTimestamp>=fact->mStartTime.addSecs(localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE()))
                 {
                      MStatisticalData minuteData;
                      minuteData.code = fact->code;
                      minuteData.timestamp = fact->mStartTime.toString("yyyy-MM-dd HH:mm");
                      minuteData.dataType = "M";
                      minuteData.valueMax = fact->mValueMax;
                      minuteData.valueMin = fact->mValueMin;
                      minuteData.valueAvg = fact->mValueSum/fact->mValueCount;
                      minuteData.valueCou = fact->mValueCou;
                      if (fact->code.left(1)=="w" && fact->mValueWarningCount>0)
                      {
                          minuteData.dataFlag = "T";
                      }
                      else if (fact->code.left(1)=="a" && fact->mValueWarningCount>0 && fact->mValueWarningCount*100/25>fact->mValueCount)
                      {
                          minuteData.dataFlag = "D";
                      }
                      else if (fact->mValueWarningCount>0)
                      {
                          minuteData.dataFlag = "T";
                      }

                      BStatisticalData bsta;
                      bsta.writeToDB(minuteData);
//qDebug()<<"minuteData has been done!"<<endl;
                      //fact->mStartTime = fact->valueTimestamp;
                      fact->mStartTime = fact->mStartTime.addSecs(localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE());
                      fact->mValueMax = fact->value;
                      fact->mValueMin = fact->value;
                      fact->mValueSum = fact->value;
                      fact->mValueCount = 1;
                      if (fact->dataFlag!="N")
                          fact->mValueWarningCount = 1;
                      else
                          fact->mValueWarningCount = 0;
                      if (w00000!=nullptr)
                      {
                          if (fact->code.left(1)=="w" && fact->code!="w00000")
                          {
                              fact->mValueCou = w00000->value * fact->value * localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE();
                          }
                          else if (fact->code=="w00000")
                          {
                              fact->mValueCou = fact->value * localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE();
                          }
                      }
                 }

                 mValueTimestamp = QDateTime::currentDateTime();
             }
        }

    }

    //qDebug()<<"updateFactorDataItem......"<<endl;
    //发送更新界面的消息
    emit updateFactorDataItem( );

    //this->isRunning=false;
}

CTask &CTask::operator=(const CTask &task)
{
    this->task=task.task;
    this->factor = task.factor;
    this->interval = task.interval;
    this->lastTime = task.lastTime;
}
