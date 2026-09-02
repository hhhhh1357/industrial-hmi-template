#include "dialogsetuptimer.h"
#include "ui_dialogsetuptimer.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <Headers/BLL/BHistoryData.h>

#include <Sources/Tools/sysutil.h>


DialogSetupTimer::DialogSetupTimer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupTimer)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");

    ui->timeEnd->setDateTime( QDateTime::currentDateTime());

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    ui->btnClose_4->setFocus();
}

DialogSetupTimer::~DialogSetupTimer()
{
    delete ui;
}

void DialogSetupTimer::on_btnClose_4_clicked()
{
    close();
}

void DialogSetupTimer::on_btnCheck_4_clicked()
{
   QDateTime dt = ui->timeEnd->dateTime();
   bool r = SysUtil::setDatetime(dt);
   if (r)
   {
       QMessageBox messageBox(QMessageBox::NoIcon, "提示信息", "时间设置成功！", QMessageBox::Ok, nullptr);
       messageBox.exec();
   }
   else
   {
       QMessageBox messageBox(QMessageBox::Warning, "警告", "时间设置失败！！！", QMessageBox::Ok, nullptr);
       messageBox.exec();
   }
}
