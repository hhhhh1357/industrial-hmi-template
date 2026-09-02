#include "dialogsetuprs485detection.h"
#include "mainwindow.h"
#include "ui_dialogsetuprs485detection.h"


extern MainWindow * mainWindow;

DialogSetupRS485Detection::DialogSetupRS485Detection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupRS485Detection)
{
    ui->setupUi(this);  
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

     keyBoard = new KeyBoard(this);
    //设置只能输入数字和小数点
    QRegExp rx("[0-9.0-9]*");
    pReg = new QRegExpValidator(rx,this);
    ui->dataReAddr->setValidator(pReg);
    ui->dataSlAddr->setValidator(pReg);

    keyBoard->changeStyle(2);
    keyBoard->changeType("符");

    ui->btnClose->setFocus();
}

DialogSetupRS485Detection::~DialogSetupRS485Detection()
{
    delete ui;
    delete pReg;
    if (keyBoard!=nullptr)
       delete keyBoard;
}

void DialogSetupRS485Detection::on_btnClose_clicked()
{
    close();
}

void DialogSetupRS485Detection::initComponents(int port)
{

}

bool DialogSetupRS485Detection::getSERIAL_PORT_CONFIG(QString portName, SERIAL_PORT_CONFIG &portConfig)
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

void DialogSetupRS485Detection::on_btnCheck_clicked()
{
    SERIAL_PORT_CONFIG portConfig;
//    unsigned char slave_address;
//    slave_address = (unsigned char)ui->dataSlAddr->text().toInt();
//    unsigned short reg_address = (unsigned short)ui->dataReAddr->text().toInt();
//    SamplerState::RS485Check(portName)
 //   bool r = true;
    if (ui->rb485Port1->isChecked())
    {
        getSERIAL_PORT_CONFIG("RS485-1",portConfig);
        QString str = SamplerState::RS485Check((URT_PORT)5, (unsigned char)ui->dataSlAddr->text().toInt(),(unsigned short)ui->dataReAddr->text().toInt(),portConfig);
        ui->dataReturn->setText(str);
    }
    else if (ui->rb485Port2->isChecked())
    {
       getSERIAL_PORT_CONFIG("RS485-2",portConfig);
       QString str = SamplerState::RS485Check((URT_PORT)6,(unsigned char)ui->dataSlAddr->text().toInt(),(unsigned short)ui->dataReAddr->text().toInt(), portConfig);
       ui->dataReturn->setText(str);
    }

}


























