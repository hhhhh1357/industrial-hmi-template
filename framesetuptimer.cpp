#include "framesetuptimer.h"
#include "ui_framesetuptimer.h"
#include "mainwindow.h"
#include "dialogsetuptimer.h"
//#include "framesetupselcheck.h"
#include "dialogsetupaicheck.h"
#include "dialogsetupfacsetting.h"
#include "dialogsetuppwd.h"
#include "dialogsetuprs232detection.h"
#include "dialogsetuprs485detection.h"
#include "dialogsetupselcheck.h"
#include "dialogsetuptouchcheck.h"



extern MainWindow * mainWindow;

FrameSetupTimer::FrameSetupTimer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameSetupTimer)
{
    ui->setupUi(this);

    this->setLayout(ui->gridLayout);

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
}

FrameSetupTimer::~FrameSetupTimer()
{
    delete ui;
}

void FrameSetupTimer::on_btnRS232Check_clicked()
{
    QDialog *dialog = new DialogSetupRS232Detection();
    dialog->show();
}

void FrameSetupTimer::on_btnRS485Check_clicked()
{
    QDialog *dialog = new DialogSetupRS485Detection();
    dialog->show();
}

void FrameSetupTimer::on_btnAiChick_clicked()
{
    QDialog *dialog = new DialogSetupAiCheck();
    dialog->show();
}

