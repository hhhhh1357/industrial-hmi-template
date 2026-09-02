#include "dialogsetuprs232detection.h"
#include "mainwindow.h"
#include "ui_dialogsetuprs232detection.h"


extern MainWindow * mainWindow;

DialogSetupRS232Detection::DialogSetupRS232Detection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupRS232Detection)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
}

DialogSetupRS232Detection::~DialogSetupRS232Detection()
{
    delete ui;
}

void DialogSetupRS232Detection::on_btnClose_clicked()
{
    close();
}

void DialogSetupRS232Detection::on_btnCheck_clicked()
{
    SERIAL_PORT_CONFIG portConfig;
    bool r = true;
    if (ui->rb232Port1->isChecked())
    {
        getSERIAL_PORT_CONFIG("RS232-1",portConfig);
        r=samplerstate.RS232Check((URT_PORT)0, portConfig);
    }
    else if (ui->rb232Port2->isChecked())
    {
        getSERIAL_PORT_CONFIG("RS232-2",portConfig);
        r=samplerstate.RS232Check((URT_PORT)1, portConfig);
    }
    else if (ui->rb232Port3->isChecked())
    {
        getSERIAL_PORT_CONFIG("RS232-3",portConfig);
        r=samplerstate.RS232Check((URT_PORT)2, portConfig);
    }
    else if (ui->rb232Port4->isChecked())
    {
        getSERIAL_PORT_CONFIG("RS232-4",portConfig);
        r=samplerstate.RS232Check((URT_PORT)3, portConfig);
    }
    else
    {
        getSERIAL_PORT_CONFIG("RS232-5",portConfig);
        r=samplerstate.RS232Check((URT_PORT)4, portConfig);
    }

    if(r)
    {
        ui->CheckData->setText("正常");
    }
    else
    {
        ui->CheckData->setText("异常");
    }


}

bool DialogSetupRS232Detection::getSERIAL_PORT_CONFIG(QString portName, SERIAL_PORT_CONFIG &portConfig)
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










