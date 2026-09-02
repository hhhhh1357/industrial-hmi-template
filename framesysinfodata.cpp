#include "framesysinfodata.h"
#include "mainwindow.h"
#include "ui_framesysinfodata.h"

#include <Headers/BLL/BLocalService.h>

#include <Sources/DT/plc.h>

extern MainWindow * mainWindow;
extern BLocalService * localService;
extern PLC plc;

FrameSysInfoData::FrameSysInfoData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameSysInfoData)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    //连接发往 PLC 命令 的信号 和 槽
    connect(this, SIGNAL(requestPLC(Task)), &plc, SLOT(requestHandler(Task)));

    listFactors();
    listConfig();
    setRefreshWarningTimer();
}

FrameSysInfoData::~FrameSysInfoData()
{
    disconnect();
    delete ui;
    delete refreshWarningTimer;
}

void FrameSysInfoData::listFactors()
{
    for (int i=0;i<ui->tableFactors->rowCount();i++)
    {
        ui->tableFactors->setItem(i, 0, &factorItem[i][0]);
        ui->tableFactors->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableFactors->setItem(i, 1, &factorItem[i][1]);
        ui->tableFactors->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    for (int i=0;i<mainWindow->factorList.count();i++)
    {
        for (int j=0;j<mainWindow->factorList.at(i)->count();j++)
        {
            if (mainWindow->factorList.at(i)->at(j)->flag<=1)
                factorItem[j][mainWindow->factorList.at(i)->at(j)->flag].setText(mainWindow->factorList.at(i)->at(j)->chName);
        }
    }
}

void FrameSysInfoData::listConfig()
{
    for (int i=0;i<ui->tableConfig->rowCount();i++)
    {
        ui->tableConfig->setItem(i, 1, &configItem[i]);
        ui->tableConfig->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    configItem[0].setText(QString::number(localService->sysConfig.SysDic.getINTERVAL())+"秒");
    configItem[1].setText(QString::number(localService->sysConfig.SysDic.getDATA_UPLOAD_REAL())+"秒");
    configItem[2].setText(QString::number(localService->sysConfig.SysDic.getDATA_UPLOAD_MINUTE()/60)+"分钟");
    configItem[3].setText(QString::number(localService->sysConfig.SysDic.getDATA_UPLOAD_HOUR()/3600)+"小时");
    configItem[4].setText(QString::number(localService->sysConfig.SysDic.getDATA_UPLOAD_DAY())+"时");

    QString tm="";
    if (localService->sysConfig.EthernetList.at(0)->status!=0||localService->sysConfig.EthernetList.at(1)->status!=0)
        tm="以太网";
    if (localService->sysConfig.dtu4G.enabled)
    {
        if (tm.length()>0)
            tm +=" + 4G-DTU";
        else
            tm = "4G-DTU";
    }

    if (tm.length()>0)
        configItem[5].setText(tm);
    else
        configItem[5].setText("网络未配置");

    configItem[6].setText(localService->sysConfig.ServerList.at(0)->ip);
    configItem[7].setText(localService->sysConfig.ServerList.at(1)->ip);
}

void FrameSysInfoData::setRefreshWarningTimer()
{
    this->refreshWarningTimer = new QTimer(this);
    connect(this->refreshWarningTimer,&QTimer::timeout,[=](){

        getWarningInfo();

        });

    this->refreshWarningTimer->setInterval(1000);  //设置定时器启动的时间间隔为 0.5 秒
    if (this->refreshWarningTimer->isActive() == false)
    {
        this->refreshWarningTimer->start();
    }
}

void FrameSysInfoData::getWarningInfo()
{
    this->warningCount=0;
    MFactor * fact;
    for (int k=0;k<mainWindow->factorList.count();k++)
    {
        QList<MFactor*> * fList;
        fList = mainWindow->factorList.at(k);
        int i;
        for (i=0;i<fList->count();i++)
        {
            fact = fList->at(i);
            if (fact->flag==0 && fact->warningCount>0)
                this->warningCount += fact->warningCount;
        }
    }

    QString style="background-color: transparent;border-image: url();";
    if (this->warningCount>0)
    {
        style += "color:red;";
        ui->lbWarningInfo->setText("监测数据有超限");
        ui->btnWP->setEnabled(true);
    }
    else
    {
        style += "color:#00FF00;";
        ui->lbWarningInfo->setText("监测数据正常");
        ui->btnWP->setEnabled(false);
    }
    ui->lbWarningInfo->setStyleSheet(style);
}

//告警处理
void FrameSysInfoData::on_btnWP_clicked()
{
    Task task;
    task.req.taskBegin = QDateTime::currentDateTime();

    //触发继电器动作
    int ch1, ch2;
    BComArgs bca;
    bool r = bca.getUsingRelayPort(ch1,ch2);
    if (r)
    {
        task.req.port = (URT_PORT)15;
        task.req.ch1 = (RELAYER_CHANNEL) ch1;
        task.req.ch2 = (RELAYER_CHANNEL) ch2;
        task.req.logic = RELAYER_LOGIC::NEGATIVE;
        task.req.action=PLCAction::CMD_WARNING_RESET;
        emit requestPLC(task);
    }
}
