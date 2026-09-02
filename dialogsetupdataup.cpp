#include "dialogsetupdataup.h"
#include "mainwindow.h"
#include "ui_dialogsetupdataup.h"

#include <Headers/BLL/BSysDictionary.h>

extern MainWindow * mainWindow;

DialogSetupDataUp::DialogSetupDataUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupDataUp)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    keyBoard = new KeyBoard(this);

    //设置只能输入数字和小数点
    QRegExp rx("[0-9]*");
    pReg = new QRegExpValidator(rx,this);

    ui->DATA_UPLOAD_REAL->setValidator(pReg);
    ui->DATA_UPLOAD_MINUTE->setValidator(pReg);
    ui->DATA_UPLOAD_HOUR->setValidator(pReg);
    ui->DATA_UPLOAD_DAY->setValidator(pReg);

    initComponents();

    ui->btnClose->setFocus();
}

DialogSetupDataUp::~DialogSetupDataUp()
{
    delete ui;

    delete keyBoard;
    delete pReg;
}



void DialogSetupDataUp::on_btnClose_clicked()
{
    close();
}

void DialogSetupDataUp::initComponents()
{
    sysDic.getSysDictionary();
    if (sysDic.getDATA_UPLOAD_REAL_ENABLED())
    {
        ui->rbYes_Real->setChecked(true);
        ui->DATA_UPLOAD_REAL->setEnabled(true);
        ui->DATA_UPLOAD_REAL->setText(QString::number(sysDic.getDATA_UPLOAD_REAL()));
    }
    else
    {
        ui->rbNo_Real->setChecked(true);
        ui->DATA_UPLOAD_REAL->setEnabled(false);
        ui->DATA_UPLOAD_REAL->setText("");
    }

    if (sysDic.getDATA_UPLOAD_MINUTE_ENABLED())
    {
        ui->rbYes_Min->setChecked(true);
        ui->DATA_UPLOAD_MINUTE->setEnabled(true);
        ui->DATA_UPLOAD_MINUTE->setText(QString::number(sysDic.getDATA_UPLOAD_MINUTE()));
    }
    else
    {
        ui->rbNo_Min->setChecked(true);
        ui->DATA_UPLOAD_MINUTE->setEnabled(false);
        ui->DATA_UPLOAD_MINUTE->setText("");
    }

    if (sysDic.getDATA_UPLOAD_HOUR_ENABLED())
    {
        ui->rbYes_Hour->setChecked(true);
        ui->DATA_UPLOAD_HOUR->setEnabled(true);
        ui->DATA_UPLOAD_HOUR->setText(QString::number(sysDic.getDATA_UPLOAD_HOUR()));
    }
    else
    {
        ui->rbNo_Hour->setChecked(true);
        ui->DATA_UPLOAD_HOUR->setEnabled(false);
        ui->DATA_UPLOAD_HOUR->setText("");
    }

    if (sysDic.getDATA_UPLOAD_DAY_ENABLED())
    {
        ui->rbYes_Day->setChecked(true);
        ui->DATA_UPLOAD_DAY->setEnabled(true);
        ui->DATA_UPLOAD_DAY->setText(QString::number(sysDic.getDATA_UPLOAD_DAY()));
    }
    else
    {
        ui->rbNo_Day->setChecked(true);
        ui->DATA_UPLOAD_HOUR->setEnabled(false);
        ui->DATA_UPLOAD_HOUR->setText("");
    }
}

void DialogSetupDataUp::on_rbYes_Real_clicked()
{
    ui->DATA_UPLOAD_REAL->setEnabled(true);
    ui->DATA_UPLOAD_REAL->setText(QString::number(sysDic.getDATA_UPLOAD_REAL()));
}

void DialogSetupDataUp::on_rbNo_Real_clicked()
{
    ui->DATA_UPLOAD_REAL->setEnabled(false);
    ui->DATA_UPLOAD_REAL->setText("");
}

void DialogSetupDataUp::on_rbYes_Min_clicked()
{
    ui->DATA_UPLOAD_MINUTE->setEnabled(true);
    ui->DATA_UPLOAD_MINUTE->setText(QString::number(sysDic.getDATA_UPLOAD_MINUTE()));
}

void DialogSetupDataUp::on_rbNo_Min_clicked()
{
    ui->DATA_UPLOAD_MINUTE->setEnabled(false);
    ui->DATA_UPLOAD_MINUTE->setText("");
}

void DialogSetupDataUp::on_rbYes_Hour_clicked()
{
    ui->DATA_UPLOAD_HOUR->setEnabled(true);
    ui->DATA_UPLOAD_HOUR->setText(QString::number(sysDic.getDATA_UPLOAD_HOUR()));
}

void DialogSetupDataUp::on_rbNo_Hour_clicked()
{
    ui->DATA_UPLOAD_HOUR->setEnabled(false);
    ui->DATA_UPLOAD_HOUR->setText("");
}

void DialogSetupDataUp::on_rbYes_Day_clicked()
{
    ui->DATA_UPLOAD_DAY->setEnabled(true);
    ui->DATA_UPLOAD_DAY->setText(QString::number(sysDic.getDATA_UPLOAD_DAY()));
}

void DialogSetupDataUp::on_rbNo_Day_clicked()
{
    ui->DATA_UPLOAD_DAY->setEnabled(false);
    ui->DATA_UPLOAD_DAY->setText("");
}

void DialogSetupDataUp::on_btnSave_clicked()
{
    bool r = true;
    if (ui->rbYes_Real->isChecked())
    {
        r = r&&sysDic.putDATA_UPLOAD_REAL(ui->DATA_UPLOAD_REAL->text().toInt());
        r = r&&sysDic.putDATA_UPLOAD_REAL_ENABLED(true);
    }
    else
    {
        r = r&&sysDic.putDATA_UPLOAD_REAL_ENABLED(false);
    }

    if(ui->rbYes_Min->isChecked())
    {
        r = r&&sysDic.putDATA_UPLOAD_MINUTE(ui->DATA_UPLOAD_MINUTE->text().toInt());
        r = r&&sysDic.putDATA_UPLOAD_MINUTE_ENABLED(true);
    }
    else
    {
        r = r&&sysDic.putDATA_UPLOAD_MINUTE_ENABLED(false);
    }

    if(ui->rbYes_Hour->isChecked())
    {
        r = r&&sysDic.putDATA_UPLOAD_HOUR(ui->DATA_UPLOAD_HOUR->text().toInt());
        r = r&&sysDic.putDATA_UPLOAD_HOUR_ENABLED(true);
    }
    else
    {
        r = r&&sysDic.putDATA_UPLOAD_HOUR_ENABLED(false);
    }

    if(ui->rbYes_Day->isChecked())
    {
        r = r&&sysDic.putDATA_UPLOAD_DAY(ui->DATA_UPLOAD_DAY->text().toInt());
        r = r&&sysDic.putDATA_UPLOAD_DAY_ENABLED(true);
    }
    else
    {
        r = r&&sysDic.putDATA_UPLOAD_DAY_ENABLED(false);
    }

    if (r)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", "数据上传参数保存成功！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "警告", "数据上传参数保存失败！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }

    emit toReboot();

}
