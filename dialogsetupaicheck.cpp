#include "dialogsetupaicheck.h"
#include "mainwindow.h"
#include "ui_dialogsetupaicheck.h"


#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <Headers/BLL/BHistoryData.h>

extern MainWindow * mainWindow;
DialogSetupAiCheck::DialogSetupAiCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupAiCheck)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
}

DialogSetupAiCheck::~DialogSetupAiCheck()
{
    delete ui;

}


void DialogSetupAiCheck::on_btnClose_clicked()
{
    close();
}

void DialogSetupAiCheck::on_btnRead_clicked()
{
    SERIAL_PORT_CONFIG portConfig;
    MComArgs mca;
    int fd = ADS7844_Open();
    if(ui->rbPort1_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-1",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        //int fd = ADS7844_Open();
        //qDebug()<<"-------------------->"<<fd<<endl;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)0,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }
    else if(ui->rbPort2_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-2",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)1,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }
    else if(ui->rbPort3_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-3",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)2,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }
    else if(ui->rbPort4_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-4",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)3,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }
    else if(ui->rbPort5_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-5",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)4,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }
    else if(ui->rbPort6_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-6",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)5,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }
    else if(ui->rbPort7_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-7",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)6,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }
    else if(ui->rbPort8_2->isChecked())
    {
        getSERIAL_PORT_CONFIG("AI-8",portConfig);

//        if(ui->rbSignalTypeV->isChecked())
//        {
//            mca.SignalType = VOLTAGE;
//        }
//        else
//        {
//            mca.SignalType = CURRENT;
//        }
        float result=0;
        int res = ADS7844_Single_Read((ADS7844_CHANNEL)7,&result);
        result/=200;
        ui->dataReadADC->setText(QString("%1").arg(result));

    }

}

bool DialogSetupAiCheck::getSERIAL_PORT_CONFIG(QString portName, SERIAL_PORT_CONFIG &portConfig)
{
    MComArgs mca;
    BComArgs bca;
    bool r =bca.getArgsByPortname(portName,mca);
    portConfig.baud = (BAUD_RATE) mca.Speed;
    portConfig.dataBits = (DATA_BITS)mca.DataBit;
    portConfig.stopBits = (STOP_BITS)mca.StopBit;
    portConfig.checkBits = (PARITY_CHECK)mca.CheckBit;
    return r;
}
