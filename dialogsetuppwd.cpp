#include "dialogsetuppwd.h"
#include "ui_dialogsetuppwd.h"

#include <Headers/BLL/BUser.h>


DialogSetupPwd::DialogSetupPwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupPwd)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    keyBoard = new KeyBoard(this);
    keyBoard->changeStyle(2);

    ui->btnClose->setFocus();

}

DialogSetupPwd::~DialogSetupPwd()
{
    delete ui;
    delete keyBoard;
}

void DialogSetupPwd::on_btnClose_clicked()
{
    close();
}

void DialogSetupPwd::on_btnSave_clicked()
{
    if (ui->dataNewPwd->text().trimmed()=="")
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "密码不能设置为空！！！", QMessageBox::Ok, nullptr);
        messageBox.exec();
        return;
    }

    if (ui->dataNewPwd->text().trimmed()==ui->dataSureNewPwd->text().trimmed())
    {
        BUser buser;
        bool r = buser.setPassword("admin",ui->dataNewPwd->text().trimmed());
        if (r)
        {
            QMessageBox messageBox(QMessageBox::Information, "提示信息", "新密码设置成功！", QMessageBox::Ok, nullptr);
            messageBox.exec();
        }
        else
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "密码设置失败！！！", QMessageBox::Ok, nullptr);
            messageBox.exec();
        }
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "两次输入的新密码不一致！！！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }
}
