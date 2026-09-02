#include "dialogsetupinternet.h"
#include "ui_dialogsetupinternet.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <Headers/BLL/BSysDictionary.h>

DialogSetupInternet::DialogSetupInternet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupInternet)
{
    ui->setupUi(this);

    initComponents();

    keyBoard = new KeyBoard(this);
    keyBoard->changeStyle(2);
    keyBoard->changeType("符");

    //this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    //this->setLayout(ui->hLayoutMain);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    //设置只能输入数字和小数点
    QRegExp rx("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    pReg1 = new QRegExpValidator(rx,this);
    ui->dataIp1->setValidator(pReg1);
    ui->dataIp2->setValidator(pReg1);
    ui->dataIp3->setValidator(pReg1);
    ui->dataIp4->setValidator(pReg1);
    ui->dataIp5->setValidator(pReg1);

    QRegExp rxp("[0-9]*");
    pReg2 = new QRegExpValidator(rxp,this);
    ui->dataInterval->setValidator(pReg2);

    ui->btnClose->setFocus();

}

DialogSetupInternet::~DialogSetupInternet()
{
    delete ui;
    delete pReg1;
    delete pReg2;
    if (keyBoard!=nullptr)
       delete keyBoard;
}

void DialogSetupInternet::on_btnClose_clicked()
{
    close();
}

void DialogSetupInternet::on_btnSave_clicked()
{
    QString servers[5];
    int num=0;

    if (this->ui->dataInterval->text().trimmed().length()==0)
    {
        QMessageBox messageBox(QMessageBox::Warning, "信息提示", "连通测试周期未设置！", QMessageBox::Ok, NULL);
        messageBox.exec();
        return;
    }

    QString msg="";
    QString t=this->ui->dataIp1->text().trimmed();
    if (t.length()>0)
    {
        if (validator(t))
        {
            servers[num++] = t;
        }
        else
        {
            msg += "IP 地址1 格式不正确！\n";
        }
    }

    t=this->ui->dataIp2->text().trimmed();
    if (t.length()>0)
    {
        if (validator(t))
        {
            servers[num++] = t;
        }
        else
        {
            msg += "IP 地址2 格式不正确！\n";
        }
    }

    t=this->ui->dataIp3->text().trimmed();
    if (t.length()>0)
    {
        if (validator(t))
        {
            servers[num++] = t;
        }
        else
        {
            msg += "IP 地址3 格式不正确！\n";
        }
    }
    t=this->ui->dataIp4->text().trimmed();
    if (t.length()>0)
    {
        if (validator(t))
        {
            servers[num++] = t;
        }
        else
        {
            msg += "IP 地址4 格式不正确！\n";
        }
    }
    t=this->ui->dataIp5->text().trimmed();
    if (t.length()>0)
    {
        if (validator(t))
        {
            servers[num++] = t;
        }
        else
        {
            msg += "IP 地址5 格式不正确！\n";
        }
    }

    if (msg.length()>0)
    {
        QMessageBox messageBox(QMessageBox::Warning, "信息提示", msg, QMessageBox::Ok, NULL);
        messageBox.exec();
        return;
    }

    if (num==0)
    {
        QMessageBox messageBox(QMessageBox::Warning, "信息提示", "请至少配置 1 个有效的 IP 地址！", QMessageBox::Ok, NULL);
        messageBox.exec();
        return;
    }

    QJsonObject jsonObject;
    jsonObject.insert("Interval",this->ui->dataInterval->text().trimmed());
    QJsonArray jsonArray;
    for (int i=0;i<num;i++)
         jsonArray.append(servers[i]);
    QJsonDocument doc = QJsonDocument(jsonArray);
    QString serverList = doc.toJson();
    serverList = serverList.replace(QRegExp("\\n"), " ");
    jsonObject.insert("ServerList",serverList);
    doc = QJsonDocument(jsonObject);

    SysDictionary sysDic;

    if (sysDic.putPING_SERVERS(doc.toJson()))
    {
        msg = "配置保存成功！";
    }
    else
    {
        msg = "配置保存失败！";
    }

    QMessageBox messageBox(QMessageBox::Information, "信息提示", msg, QMessageBox::Ok, NULL);
    messageBox.exec();

     emit toReboot();

}

void DialogSetupInternet::initComponents()
{
    SysDictionary sysDic;
    int interval = sysDic.getPING_SERVERS_Interval();
    ui->dataInterval->setText(QString::number(interval));
    QList<QString> serverList = sysDic.getPING_SERVERS_List();

    if (serverList.length()>0)
        ui->dataIp1->setText(serverList.at(0));
    if (serverList.length()>1)
        ui->dataIp2->setText(serverList.at(1));
    if (serverList.length()>2)
        ui->dataIp3->setText(serverList.at(2));
    if (serverList.length()>3)
        ui->dataIp4->setText(serverList.at(3));
    if (serverList.length()>4)
        ui->dataIp5->setText(serverList.at(4));
}

bool DialogSetupInternet::validator(QString text)
{
    int pos=0;
    QValidator::State state = ui->dataIp1->validator()->validate(text,pos);
    if (state == QValidator::State::Acceptable)
        return true;
    else
        return false;
}
