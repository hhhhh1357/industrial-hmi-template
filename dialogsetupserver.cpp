#include "dialogsetupserver.h"
#include "ui_dialogsetupserver.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <Headers/BLL/BHistoryData.h>
#include <Headers/BLL/BSysConfig.h>

extern MainWindow * mainWindow;

DialogSetupServer::DialogSetupServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupServer)
{
    ui->setupUi(this);

    keyBoard = new KeyBoard(this);

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setLayout(ui->hLayoutMain);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    //设置只能输入数字和小数点
    QRegExp rx("[0-9.0-9]*");
    pReg = new QRegExpValidator(rx,this);
    ui->dataIp->setValidator(pReg);

    QRegExp rxp("[0-9]*");
    pReg = new QRegExpValidator(rxp,this);

    ui->dataPort->setValidator(pReg);

    ui->dataInterval->setValidator(pReg);

    BSysConfig sysConfig;
    sysConfig.getServerList(ServerList);

    initComponents();

    ui->btnClose->setFocus();
}

DialogSetupServer::~DialogSetupServer()
{
    delete ui;
    delete pReg;
    if (keyBoard!=nullptr)
       delete keyBoard;

    for (int i=0;i<ServerList.count();i++)
        delete ServerList.at(i);
}

void DialogSetupServer::initComponents()
{
   Server * server;
   if (ui->rbServer1->isChecked())
   {
       server = ServerList.at(0);
   }
   else if (ui->rbServer2->isChecked())
   {
       server = ServerList.at(1);
   }
   else if (ui->rbServer3->isChecked())
   {
       server = ServerList.at(2);
   }
   else if (ui->rbServer4->isChecked())
   {
       server = ServerList.at(3);
   }
   if (server->status!=0)
       ui->chkEnabled->setChecked(true);
   else
       ui->chkEnabled->setChecked(false);
   ui->dataIp->setText(server->ip);
   ui->dataPass->setText(server->password);
   ui->dataPort->setText(QString::number(server->port));
   ui->dataInterval->setText(QString::number(server->interval));

}

//bool DialogSetupServer::getArgs(QString argsStr, QString *IP, QString *PORT, QString *PASSWORD, int *INTERVAL)
//{
//    QJsonDocument jsonDocument = QJsonDocument::fromJson(argsStr.toLocal8Bit().data());
//    if (!(jsonDocument.isNull()))
//    {
//        QJsonObject comArgsJson = jsonDocument.object();
//        *IP = comArgsJson.value("IP").toString();
//        *PORT = comArgsJson.value("PORT").toString();
//        *PASSWORD = comArgsJson.value("PASSWORD").toString();
//        *INTERVAL = comArgsJson.value("INTERVAL").toInt();
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

//bool DialogSetupServer::saveServerArgs(MComArgs * mca, QString IP, QString PORT, QString PASSWORD, int INTERVAL)
//{
//    QJsonObject jsonObject;
//    jsonObject.insert("IP",IP);
//    jsonObject.insert("PORT",PORT);
//    jsonObject.insert("PASSWORD",PASSWORD);
//    jsonObject.insert("INTERVAL",INTERVAL);
//    auto doc = QJsonDocument(jsonObject);

//    QMutexLocker locker(&(BComArgs::mutexComArgsList));
//    mca->Args = doc.toJson();
//    mca->Status = NORMAL;
//    BComArgs bca;
//    return bca.update(mca);
//    return true;
//}

void DialogSetupServer::on_btnClose_clicked()
{
    close();
}

void DialogSetupServer::on_btnSave_clicked()
{
    bool r;
    Server * server;

    if (ui->rbServer1->isChecked())
    {
        server = ServerList.at(0);
    }
    else if (ui->rbServer2->isChecked())
    {
        server = ServerList.at(1);
    }
    else if (ui->rbServer3->isChecked())
    {
        server = ServerList.at(2);
    }
    else
    {
        server = ServerList.at(3);
    }

    server->status = ui->chkEnabled->isChecked()?1:0;
    server->ip = ui->dataIp->text().trimmed();
    server->port = ui->dataPort->text().trimmed().toInt();
    server->password = ui->dataPass->text().trimmed();
    server->interval = ui->dataInterval->text().trimmed().toInt();

    BSysConfig sysConfig;
    r = sysConfig.saveServerArgs(*server);

    //重新初始化数据
    //mainWindow->reInitialize();

    if (r)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", ""+ server->serverName+ "服务器参数保存成功！", QMessageBox::Ok, NULL);
        messageBox.exec();

    }
    else
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "警告", ""+ server->serverName+ "服务器参数保存失败！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    emit toReboot();
}

void DialogSetupServer::on_rbServer1_clicked()
{
    initComponents();
}

void DialogSetupServer::on_rbServer2_clicked()
{
    initComponents();
}

void DialogSetupServer::on_rbServer3_clicked()
{
    initComponents();
}

void DialogSetupServer::on_rbServer4_clicked()
{
    initComponents();
}

void DialogSetupServer::on_chkEnabled_stateChanged(int arg1)
{
    QString style="background-color: transparent;border-width:0;border-style:outset;border-bottom:2px solid #80d0c7;";
    if (arg1==0)
    {
        style+="color:gray;";
        ui->dataIp->setEnabled(false);
        ui->dataPass->setEnabled(false);
        ui->dataPort->setEnabled(false);
        ui->dataInterval->setEnabled(false);
    }
    else
    {
        style+="color:black;";
        ui->dataIp->setEnabled(true);
        ui->dataPass->setEnabled(true);
        ui->dataPort->setEnabled(true);
        ui->dataInterval->setEnabled(true);
    }

    ui->dataIp->setStyleSheet(style);
    ui->dataPass->setStyleSheet(style);
    ui->dataPort->setStyleSheet(style);
    ui->dataInterval->setStyleSheet(style);
}
