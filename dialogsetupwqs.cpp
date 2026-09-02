#include "dialogsetupwqs.h"
#include "mainwindow.h"
#include "ui_dialogsetupwqs.h"

#include <Headers/BLL/BComArgs.h>
#include <Headers/BLL/BFactor.h>

#include <QJsonArray>
#include <QJsonDocument>

extern MainWindow * mainWindow;

DialogSetupWQS::DialogSetupWQS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupWQS)
{
    ui->setupUi(this);

     this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");

    keyBoard = new KeyBoard(this);

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    if (mainWindow->wqs==nullptr)
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "请先在 RS232 或 RS485 配置水质采样仪！", QMessageBox::Ok, NULL);
        messageBox.exec();
        this->close();
        return;
    }

    //设置PLC地址的数据输入格式：只能是数字
    QRegExp rx("[0-9]*");
    pReg = new QRegExpValidator(rx,this);
    ui->interval->setValidator(pReg);

    //判断是否配置了相关的分析仪
    QList<MFactor *> dataList = mainWindow->getDataItemList();
    for (int i=0;i<dataList.count();i++)
    {
        if (dataList.at(i)->code=="w01018")
        {
           ui->checkBox_w01018->setEnabled(true);
        }
        else if (dataList.at(i)->code=="w21001")
        {
           ui->checkBox_w21001->setEnabled(true);
        }
        else if (dataList.at(i)->code=="w21003")
        {
           ui->checkBox_w21003->setEnabled(true);
        }
        else if (dataList.at(i)->code=="w21011")
        {
           ui->checkBox_w21011->setEnabled(true);
        }
    }

    //读取数据库配置数据
    BFactor bf;
    MFactor factor;

    bf.getFactorInfoByCode(WQS,factor);

    ui->lbPortName->setText(factor.dataPortName);
    ui->interval->setText(QString::number(factor.wqs.Interval));

    printf("factor.wqs.strProtocol: %s\n", factor.wqs.strProtocol);
    ui->cmbProtocol->setCurrentText(factor.wqs.strProtocol);
    //ui->cmbProtocol->setCurrentIndex();

    for (int i=0;i<factor.wqs.Factors.count();i++)
    {
        if (factor.wqs.Factors.at(i)=="w01018")
        {
           ui->checkBox_w01018->setChecked(true);
        }
        else if (factor.wqs.Factors.at(i)=="w21001")
        {
           ui->checkBox_w21001->setChecked(true);
        }
        else if (factor.wqs.Factors.at(i)=="w21003")
        {
           ui->checkBox_w21003->setChecked(true);
        }
        else if (factor.wqs.Factors.at(i)=="w21011")
        {
           ui->checkBox_w21011->setChecked(true);
        }
    }
}

DialogSetupWQS::~DialogSetupWQS()
{
    delete ui;
    if (keyBoard!=nullptr)
        delete keyBoard;
    if (pReg!=nullptr)
        delete pReg;
}

void DialogSetupWQS::on_btnClose_clicked()
{
    close();
}

void DialogSetupWQS::on_btnSave_clicked()
{

    if (ui->interval->text().toInt()<30 ||ui->interval->text().toInt()>3600)
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "采样时间应设置在30～3600之间！", QMessageBox::Ok, NULL);
        messageBox.exec();
        return;
    }

    //读取数据库配置数据
    BFactor bf;
    MFactor factor;
    bf.getFactorInfoByCode(WQS,factor);
    factor.wqs.Interval = ui->interval->text().toInt();

    printf("ui->cmbProtocol->currentText(): %s\n", ui->cmbProtocol->currentText());
    factor.wqs.strProtocol = ui->cmbProtocol->currentText();

    factor.wqs.Factors.clear();
    if (ui->checkBox_w01018->isChecked())
        factor.wqs.Factors.append("w01018");
    if (ui->checkBox_w21001->isChecked())
        factor.wqs.Factors.append("w21001");
    if (ui->checkBox_w21003->isChecked())
        factor.wqs.Factors.append("w21003");
    if (ui->checkBox_w21011->isChecked())
        factor.wqs.Factors.append("w21011");

    bool rs  = bf.saveFactor(factor);

    //重新初始化数据
    //mainWindow->reInitialize();

    if (rs)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", "水质采样器配置参数保存成功！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "水质采样器配置参数保存失败！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    emit toReboot();
}
