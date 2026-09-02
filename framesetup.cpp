#include "framesetup.h"
#include "ui_framesetup.h"

#include "framesetupcomm.h"
#include "framesetupcontrol.h"
#include "framesetupother.h"
#include "framesetuptimer.h"
#include "mainwindow.h"

extern MainWindow * mainWindow;

//FrameSetup * frameSetup;

FrameSetup::FrameSetup(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameSetup)
{
    ui->setupUi(this);

    this->setLayout(ui->horizontalLayout);

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    ButtonStytleNormal = "QToolButton{color:rgb(0, 0, 0);min-height:20;border-style:solid;border-top-left-radius:2px;border-top-right-radius:2px;";
    ButtonStytleNormal +="background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 #f3e7e9, stop: 1 #e3eeff);";
    ButtonStytleNormal += "border:1px;border-radius:5px;padding:2px 4px;}";

    ButtonStytlePresed = "QToolButton{color:rgb(0, 0, 0);min-height:20;border-style:solid;border-top-left-radius:2px;border-top-right-radius:2px;";
    ButtonStytlePresed +="background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 #ff5858, stop: 1 #f09819);";
    ButtonStytlePresed += "border:1px;border-radius:5px;padding:2px 4px;}";

    initAllButtonsStyle();

    mainWindow->stopReadDataFromPLC();  //端口配置，禁止从PLC读数据
    mainWindow->stopRefreshDataCom();   //端口配置，禁止刷新端口状态

    on_btn4_clicked();
}

FrameSetup::~FrameSetup()
{
    if (forceReboot==false)  //不用重启
    {
        //恢复采集数据等
        mainWindow->startReadDataFromPLC(); //启动从PCL读取数据
        mainWindow->startRefreshDataCom(); //启动刷新端口状态
    }
//    else  //强制重启
//    {
//        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", "重要配置参数修改了，系统将重新启动！！！", QMessageBox::Ok, nullptr);
//        messageBox.exec();

//        mainWindow->reboot();
//    }
    delete ui;
    if (currFrame!=nullptr) delete currFrame;
}

void FrameSetup::initAllButtonsStyle()
{
    ui->btn1->setStyleSheet(ButtonStytleNormal);
    ui->btn4->setStyleSheet(ButtonStytleNormal);
//*********************************20220617屏蔽反控界面***************************************//
   // ui->btnControl->setStyleSheet(ButtonStytleNormal);
    ui->btnOther->setStyleSheet(ButtonStytleNormal);
}

void FrameSetup::on_btn1_clicked()//设备自检
{
    initAllButtonsStyle();
    ui->btn1->setStyleSheet(ButtonStytlePresed);
    if (currFrame!=nullptr) delete currFrame;
    currFrame = new FrameSetupTimer(ui->frame);
    currFrame->show();
}

void FrameSetup::on_btn4_clicked() //通讯设置
{
    initAllButtonsStyle();
    ui->btn4->setStyleSheet(ButtonStytlePresed);

    if (currFrame!=nullptr) delete currFrame;

    currFrame = new FrameSetupComm(ui->frame);
/****************************判断信号槽是否连接******************************/
 //   if(comReBoot == false)
       connect(currFrame, SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    currFrame->show();

}

/****************************20220617屏蔽反控制界面********************/
//void FrameSetup::on_btnControl_clicked()
//{
//    initAllButtonsStyle();
//    ui->btnControl->setStyleSheet(ButtonStytlePresed);
//    if (currFrame!=nullptr) delete currFrame;

//    currFrame = new FrameSetupControl(ui->frame);
//    currFrame->show();
//}

void FrameSetup::on_btnOther_clicked() //其它设置
{
    initAllButtonsStyle();
    ui->btnOther->setStyleSheet(ButtonStytlePresed);
    if (currFrame!=nullptr) delete currFrame;

    currFrame = new FrameSetupOther(ui->frame);
/****************************判断信号槽是否连接******************************/
//    if(comReBoot == false)
       connect(currFrame, SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    currFrame->show();

}

void FrameSetup::setForceReboot()
{
    this->forceReboot = true;    

    mainWindow->isRboot = true;
}
