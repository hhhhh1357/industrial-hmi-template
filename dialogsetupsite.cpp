#include "dialogsetupsite.h"
#include "ui_dialogsetupsite.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <Headers/BLL/BHistoryData.h>

extern MainWindow * mainWindow;

DialogSetupSite::DialogSetupSite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupSite)
{
    ui->setupUi(this);
    keyBoard = new KeyBoard(this);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setLayout(ui->hLayoutMain);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    //设置只能输入数字和小数点
    QRegExp rx("[0-9]*");
    pReg = new QRegExpValidator(rx,this);
    ui->dataMN->setValidator(pReg);
    ui->dataTIMEOUT->setValidator(pReg);
    ui->dataREPEAT->setValidator(pReg);
    ui->dataSTORAGE_TIME->setValidator(pReg);
    ui->dataINTERVAL->setValidator(pReg);

    initComponents();

    ui->btnClose->setFocus();
}

DialogSetupSite::~DialogSetupSite()
{
    delete ui;
    delete pReg;
    if (keyBoard!=nullptr)
       delete keyBoard;
}

void DialogSetupSite::initComponents()
{
     SysDictionary sysDic;
     ui->dataMN->setText(sysDic.getMN());    
     ui->dataST->setText(sysDic.getST());
     ui->dataTIMEOUT->setText(QString::number(sysDic.getINTERVAL()));
     ui->dataREPEAT->setText(QString::number(sysDic.getREPEAT()));
     ui->dataSTORAGE_TIME->setText(QString::number(sysDic.getSTORAGE_TIME()));
     ui->dataINTERVAL->setText(QString::number(sysDic.getINTERVAL()));

}

void DialogSetupSite::on_btnClose_clicked()
{
    close();
}

void DialogSetupSite::on_btnSave_clicked()
{
    SysDictionary sysDic;
    if (ui->dataMN->text().trimmed().length()<=0)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "警告", "设备号不能为空！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    bool ena=true;
//    if (ui->rbNo->isChecked())
//        ena=false;

    bool r=true;
    r = r && sysDic.putMN(ui->dataMN->text().trimmed());
    r = r && sysDic.putST(ui->dataST->text().trimmed());
    r = r && sysDic.putTIMEOUT(ui->dataTIMEOUT->text().toInt());
    r = r && sysDic.putREPEAT(ui->dataREPEAT->text().toInt());
    r = r && sysDic.putINTERVAL(ui->dataINTERVAL->text().toInt());
    r = r && sysDic.putSTORAGE_TIME(ui->dataSTORAGE_TIME->text().toInt());

    //重新初始化数据
    //mainWindow->reInitialize();

    if (r)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", "站点参数保存成功！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "警告", "站点参数保存失败！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }

    emit toReboot();
}
