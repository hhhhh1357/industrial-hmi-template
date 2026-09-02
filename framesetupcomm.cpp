#include "framesetupcomm.h"
#include "ui_framesetupcomm.h"
#include "dialogsetupcom.h"
#include "dialogsetupai.h"

#include "dialogsetupdataup.h"
#include "dialogsetupenvironment.h"
#include "dialogsetupnet.h"
#include "dialogsetuprelay.h"
#include "dialogsetupserver.h"
#include "dialogsetupsite.h"
#include "dialogsetupswitch.h"
#include "dialogsetupwqs.h"
#include "framesetup.h"
#include "mainwindow.h"

extern MainWindow * mainWindow;
//extern FrameSetup * frameSetup;


FrameSetupComm::FrameSetupComm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameSetupComm)
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout);

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
}

FrameSetupComm::~FrameSetupComm()
{
    delete ui;
}


void FrameSetupComm::on_btnRS232_clicked()
{
    QDialog * dialog = new DialogSetupCom("RS232");
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnRS485_clicked()
{
    QDialog * dialog = new DialogSetupCom("RS485");
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnAI_clicked()
{
    QDialog * dialog = new DialogSetupAI("AI");
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnSwitch_clicked()
{
    QDialog * dialog = new DialogSetupSwitch("SW");
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnRelay_clicked()
{
    QDialog * dialog = new DialogSetupRelay("RELAY");
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnWater_clicked()
{
    QDialog * dialog = new DialogSetupWQS();
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}


void FrameSetupComm::on_btnServer_clicked()
{
    QDialog * dialog =new DialogSetupServer();
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnNetwork_clicked()
{
    QDialog * dialog =new DialogSetupNet();
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnSite_clicked()
{
    QDialog * dialog =new DialogSetupSite();
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::on_btnDataUp_clicked()
{
    QDialog * dialog =new DialogSetupDataUp();
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupComm::setForceReboot()
{
//    frameSetup->comReBoot = true;
    emit toReboot();
}
