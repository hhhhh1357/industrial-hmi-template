#include "dialogsetupfacsetting.h"
#include "dialogsetuppwd.h"
#include "dialogsetupscreensavetime.h"
#include "dialogsetuptimer.h"
#include "dialogsetupwatchdog.h"
#include "dialogsetupinternet.h"
#include "framesetupother.h"
#include "ui_framesetupother.h"

FrameSetupOther::FrameSetupOther(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameSetupOther)
{
    ui->setupUi(this);
}

FrameSetupOther::~FrameSetupOther()
{
    delete ui;
}

void FrameSetupOther::on_btnTimerSet_clicked()
{
    QDialog *dialog = new DialogSetupTimer();
    dialog->show();
}

void FrameSetupOther::on_btnPwdSet_clicked()
{
    QDialog *dialog = new DialogSetupPwd();
    dialog->show();
}

void FrameSetupOther::on_btnDogSet_clicked()
{
    QDialog *dialog = new DialogSetupWatchDog();
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupOther::on_btnReFacSet_clicked()
{
    QDialog *dialog = new DialogSetupFacSetting();
    dialog->show();
}

void FrameSetupOther::on_btnScreenTIme_clicked()
{
    QDialog *dialog = new DialogSetupScreenSaveTime();
    connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}

void FrameSetupOther::setForceReboot()
{
    emit toReboot();
}

void FrameSetupOther::on_btnPortRe_clicked()
{

}


void FrameSetupOther::on_btnInternetServerSet_clicked()
{
    QDialog *dialog = new DialogSetupInternet();
     connect(dialog,SIGNAL(toReboot()),this,SLOT(setForceReboot()));
    dialog->show();
}
