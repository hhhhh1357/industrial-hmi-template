#include "dialogsetupswitch.h"
#include "ui_dialogsetupswitch.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStyle>

extern MainWindow * mainWindow;
DialogSetupSwitch::DialogSetupSwitch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupSwitch)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);//设置顶层窗口无边框

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setLayout(ui->hLayoutMain);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    this->bca.queryDataComArgsList(&(this->bca.ComArgsListNew));
}

DialogSetupSwitch::DialogSetupSwitch(QString comType, QWidget *parent):DialogSetupSwitch(parent)
{
    this->SwitchType = comType ;
    initComponents(this->SwitchType,1); //显示端口1
}

DialogSetupSwitch::~DialogSetupSwitch()
{
    delete ui;
    delete  this->mca;
}

void DialogSetupSwitch::initComponents(QString SwitchType, int port)
{
//    QString portName = SwitchType + "-"+QString::number(port);
//    int i=0;
//    QMutexLocker locker1(&(BComArgs::mutexComArgsList));
//    for (;i<BComArgs::ComArgsList.count();i++)
//    {
//        if ( BComArgs::ComArgsList.at(i)->PortName == portName )
//            break;
//    }
//    if (i>=BComArgs::ComArgsList.count()) return;

//    this->mca = new MComArgs();

//    *(this->mca) = *(BComArgs::ComArgsList.at(i));

//    if (BComArgs::ComArgsList.at(i)->Status!=UNUSED)
//    {
//        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Checked);
//        setParamentComponentsEnable(true);
//    }
//    else
//    {
//        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Unchecked);
//        setParamentComponentsEnable(false);
//    }

    QString portName = SwitchType + "-"+QString::number(port);
    int i;

    for (i=0;i<this->bca.ComArgsListNew.count();i++)
    {
        if ( this->bca.ComArgsListNew.at(i)->PortName == portName )
            break;
    }
    if (i>=this->bca.ComArgsListNew.count()) return;

    this->mca = new MComArgs();

    *(this->mca) = *(this->bca.ComArgsListNew.at(i));

    if (this->bca.ComArgsListNew.at(i)->Status!=UNUSED)
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Checked);
        setParamentComponentsEnable(true);
    }
    else
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Unchecked);
        setParamentComponentsEnable(false);
    }
}

void DialogSetupSwitch::setParamentComponentsEnable(bool enable)
{
    QString color;
    QString style;
//    ui->rbLogicP->setEnabled(enable);
//    ui->rbLogicE->setEnabled(enable);
//    ui->lbLogic->setEnabled(enable);

    if (enable)
    {
        color = "black";
    }
    else
    {
        color = "gray";
    }

    style = "QRadioButton::indicator{width: 24px;height: 24px;border-radius:13px;border-width:2px;}";
    style += "QRadioButton::indicator::unchecked {background-color:transparent;border:2px solid black;border-radius:13px;}";
    style += "QRadioButton::indicator::checked {background-color:green;border:2px solid black;border-radius:13px;}";
    style += "QRadioButton{background-color: transparent;color:"+color+";}";

//    ui->rbLogicP->setStyleSheet(style);
//    ui->rbLogicE->setStyleSheet(style);

    style = "background-color: transparent;color:"+color+";";
//    ui->lbLogic->setStyleSheet(style);
}

void DialogSetupSwitch::on_rbPort1_clicked()
{
    initComponents(this->SwitchType,1);
}

void DialogSetupSwitch::on_btnClose_clicked()
{
    close();
}

void DialogSetupSwitch::on_rbPort2_clicked()
{
     initComponents(this->SwitchType,2);

}

void DialogSetupSwitch::on_rbPort3_clicked()
{
     initComponents(this->SwitchType,3);

}

void DialogSetupSwitch::on_rbPort4_clicked()
{
     initComponents(this->SwitchType,4);
}


void DialogSetupSwitch::on_btnSave_clicked()
{

    Qt::CheckState s = ui->checkBoxPortEnabled->checkState();

    if (s == Qt::CheckState::Unchecked)
        mca->Status = UNUSED;
    else
        mca->Status = NORMAL;

    if (mca->Status == UNUSED)
    {
        mca->Args="";
    }
    else
    {
        QJsonObject jsonObject;
        jsonObject.insert("LogicType",mca->LogicType);
        jsonObject.insert("FactorArgs","");
        QJsonDocument doc = QJsonDocument(jsonObject);
        mca->Args = doc.toJson();
    }
    //BComArgs bca;
    bool rr = this->bca.update(this->mca);

    if (rr)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", this->mca->PortName + " 参数保存成功！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "警告", this->mca->PortName + " 参数保存失败！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    emit toReboot();
}

void DialogSetupSwitch::on_checkBoxPortEnabled_stateChanged(int arg1)
{
    if (mca->Status!=UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Unchecked)
    {
        if (QMessageBox::No==QMessageBox::question(nullptr,tr("提示"),tr("确定此端口不使用吗？"),
                                                    QMessageBox::Yes|QMessageBox::No,
                QMessageBox::No))
        {
             ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Checked);
        }
        else
        {
            // 相关组件设置成 无效
            setParamentComponentsEnable(false);
            ui->checkBoxPortEnabled->setText("未启用");
        }
    }
    else if (mca->Status!=UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Checked)
    {
         setParamentComponentsEnable(true);
         ui->checkBoxPortEnabled->setText("已启用");
    }
    else if (mca->Status==UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Unchecked)
    {
        // 相关组件设置成 无效
       setParamentComponentsEnable(false);
        ui->checkBoxPortEnabled->setText("未启用");
    }
    else
    {
        setParamentComponentsEnable(true);
        ui->checkBoxPortEnabled->setText("已启用");
    }
}


