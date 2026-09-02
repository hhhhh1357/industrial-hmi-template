#include "dialogsetupscreensavetime.h"
#include "ui_dialogsetupscreensavetime.h"

#include <qmessagebox.h>

#include <Headers/BLL/BLocalService.h>
extern BLocalService * localService;

DialogSetupScreenSaveTime::DialogSetupScreenSaveTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupScreenSaveTime)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    keyBoard = new KeyBoard(this);
   //设置只能输入数字和小数点
   QRegExp rx("[0-9]*");
   pReg = new QRegExpValidator(rx,this);
   ui->leScreenTime->setValidator(pReg);

   ui->leScreenTime->setText(QString::number(localService->sysConfig.SysDic.getSCREEN_SAVE_TIME()));

   keyBoard->changeStyle(2);
   keyBoard->changeType("符");

   ui->btnClose->setFocus();
}

DialogSetupScreenSaveTime::~DialogSetupScreenSaveTime()
{
    delete ui;

    delete keyBoard;
    delete  pReg;
}

void DialogSetupScreenSaveTime::on_btnClose_clicked()
{
    close();
}

void DialogSetupScreenSaveTime::on_btnSave_clicked()
{
    if(ui->leScreenTime->text().trimmed().length()==0)
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告！",  " 屏保时间不能为空！！！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }
    else
    {
         bool r =  localService->sysConfig.SysDic.putSCREEN_SAVE_TIME(ui->leScreenTime->text().trimmed().toInt());
         if(r)
         {
             QMessageBox messageBox(QMessageBox::Information, "信息提示",  " 屏保时间设置成功！", QMessageBox::Ok, nullptr);
             messageBox.exec();
         }
          emit toReboot();
    }


}
