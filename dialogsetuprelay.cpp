#include "dialogsetuprelay.h"
#include "ui_dialogsetuprelay.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
extern MainWindow * mainWindow;
DialogSetupRelay::DialogSetupRelay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupRelay)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);//设置顶层窗口无边框

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    this->bca.queryDataComArgsList(&(this->bca.ComArgsListNew));
}

DialogSetupRelay::DialogSetupRelay(QString comType, QWidget *parent):DialogSetupRelay(parent)
{
    this->RelayType = comType ;
    initComponents(this->RelayType,1); //显示端口1
}

DialogSetupRelay::~DialogSetupRelay()
{
    delete ui;
}

void DialogSetupRelay::initComponents(QString RelayType, int port)
{
    /******************************************************************/
    QString portName = RelayType + "-"+QString::number(port);
    int i;

    for (i=0;i<this->bca.ComArgsListNew.count();i++)
    {
        if ( this->bca.ComArgsListNew.at(i)->PortName == portName )
            break;
    }
    if (i>=this->bca.ComArgsListNew.count()) return;

    this->mca = *(this->bca.ComArgsListNew.at(i));

    if (mca.Status!=UNUSED)
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Checked);
        setParamentComponentsEnable(true);
    }
    else
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Unchecked);
        setParamentComponentsEnable(false);
    }

    if (mca.StatusType == ALARM)
        ui->rbAlarm->setChecked(true);
    else
        ui->rbCount->setChecked(true);
}

void DialogSetupRelay::setParamentComponentsEnable(bool enable)
{
    /*****************************************************************/
    QString color;
    QString style;
    ui->rbAlarm->setEnabled(enable);
    ui->rbCount->setEnabled(enable);
    ui->lbStatus->setEnabled(enable);

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

    ui->rbAlarm->setStyleSheet(style);
    ui->rbCount->setStyleSheet(style);


    style = "background-color: transparent;color:"+color+";";
    ui->lbStatus->setStyleSheet(style);
}

void DialogSetupRelay::on_btnClose_clicked()
{
    close();
}

void DialogSetupRelay::on_btnSave_clicked()
{
    //QMutexLocker locker1(&(BComArgs::mutexComArgsList));

    Qt::CheckState s = ui->checkBoxPortEnabled->checkState();

    if (s == Qt::CheckState::Unchecked)
        mca.Status = UNUSED;
    else
        mca.Status = NORMAL;

    if (mca.Status == UNUSED)
    {
        mca.Args="";
    }
    else
    {
        if (ui->rbAlarm->isChecked())
            mca.StatusType = ALARM;
        else
            mca.StatusType = THREE_COUNT;

        QJsonObject jsonObject;
        jsonObject.insert("StatusType",mca.StatusType);
        jsonObject.insert("FactorArgs","");
        QJsonDocument doc = QJsonDocument(jsonObject);
        mca.Args = doc.toJson();
    }

    bool rr = this->bca.update(&(this->mca));

    //重新初始化数据
    //mainWindow->reInitialize();

    if (rr)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", this->mca.PortName + " 参数保存成功！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "警告", this->mca.PortName + " 参数保存失败！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }

    emit toReboot();

}

void DialogSetupRelay::on_rbCH1_clicked()
{
     initComponents(this->RelayType,1);
}

void DialogSetupRelay::on_rbCH2_clicked()
{
    initComponents(this->RelayType,2);
}

void DialogSetupRelay::on_rbCH3_clicked()
{
    initComponents(this->RelayType,3);
}

void DialogSetupRelay::on_rbCH4_clicked()
{
    initComponents(this->RelayType,4);
}


void DialogSetupRelay::on_checkBoxPortEnabled_stateChanged(int arg1)
{
    if (mca.Status!=UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Unchecked)
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
    else if (mca.Status!=UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Checked)
    {
         setParamentComponentsEnable(true);
         ui->checkBoxPortEnabled->setText("已启用");
    }
    else if (mca.Status==UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Unchecked)
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
