#include "dialogsetupwatchdog.h"
#include "ui_dialogsetupwatchdog.h"

#include <qmessagebox.h>

#include <Headers/BLL/BLocalService.h>

extern BLocalService * localService;

DialogSetupWatchDog::DialogSetupWatchDog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupWatchDog)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    keyBoard = new KeyBoard(this);
    //设置只能输入数字和小数点
    QRegExp rx("[0-9.0-9]*");
    pReg = new QRegExpValidator(rx,this);
    ui->dataTimeSet->setValidator(pReg);

    ui->dataTimeSet->setText(QString::number(localService->sysConfig.SysDic.getDOG_TIMER()));

    ui->btnClose->setFocus();

}

DialogSetupWatchDog::~DialogSetupWatchDog()
{
    delete ui;
    delete keyBoard;
    delete pReg;
}

void DialogSetupWatchDog::on_btnClose_clicked()
{
    close();
}

void DialogSetupWatchDog::on_btnSet_clicked()
{
    if(ui->dataTimeSet->text().trimmed()==0)
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告",  "巡逻时间不能为空！！！", QMessageBox::Ok, nullptr);
        messageBox.exec();
        return;
    }

    bool r = localService->sysConfig.SysDic.putDOG_TIMER(ui->dataTimeSet->text().trimmed().toInt());

    if (r)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示",  "巡逻时间设置成功！", QMessageBox::Ok, nullptr);
        messageBox.exec();

        emit toReboot();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告",  "巡逻时间设置失败！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }

}
