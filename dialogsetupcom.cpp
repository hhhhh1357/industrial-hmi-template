#include "dialogsetupcom.h"
#include "framesetupcomm.h"
#include "ui_dialogsetupcom.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <Headers/BLL/BHistoryData.h>

extern MainWindow * mainWindow;


DialogSetupCom::DialogSetupCom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupCom)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");

    ui->widget->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    ui->frameParaments->setLayout(ui->gridLayout);

    this->bca.queryDataComArgsList(&(this->bca.ComArgsListNew));
}

DialogSetupCom::DialogSetupCom(QString comType,QWidget *parent):DialogSetupCom(parent)
{
    this->ComType = comType ;
    ui->comType->setText(comType);
    if (comType=="RS485")
    {
        ui->rbPort3->setVisible(false);
        ui->rbPort4->setVisible(false);
        ui->rbPort5->setVisible(false);
    }
    initComponents(this->ComType,1); //显示端口1
}

DialogSetupCom::~DialogSetupCom()
{
    delete ui;
}


void DialogSetupCom::on_btnClose_clicked() //关闭窗口
{
    close();
}

void DialogSetupCom::on_btnSave_clicked() //保存配置
{
   // QMutexLocker locker1(&(BComArgs::mutexComArgsList));

    Qt::CheckState s = ui->checkBoxPortEnabled->checkState();

    if (s == Qt::CheckState::Unchecked)
        mmca.Status = UNUSED;
    else
        mmca.Status = NORMAL;

    if (mmca.Status == UNUSED)
    {
        mmca.Args="";
    }
    else
    {
        mmca.Speed = ui->cmbSpeed->currentText().toInt();

        if (ui->rbDataBit7->isChecked())
            mmca.DataBit = 7;
        else
            mmca.DataBit = 8;

        if (ui->rbStopBit1->isChecked())
            mmca.StopBit = 1;
        else
            mmca.StopBit = 2;

        if (ui->rbCheckBitEven->isChecked())
            mmca.CheckBit = CHECK_EVEN;
        else if (ui->rbCheckBitOdd->isChecked())
            mmca.CheckBit = CHECK_ODD;
        else
            mmca.CheckBit = CHECK_NONE;

        QJsonObject jsonObject;
        jsonObject.insert("Speed",mmca.Speed);
        jsonObject.insert("DataBit",mmca.DataBit);
        jsonObject.insert("StopBit",mmca.StopBit);
        jsonObject.insert("CheckBit",mmca.CheckBit);

        QJsonArray jsonArray;
        for (auto & cfa : mmca.ComFactorArgsList)
             jsonArray.append(cfa.argsToJson());
        auto doc = QJsonDocument(jsonArray);
        QString argsStr = doc.toJson();
        argsStr = argsStr.replace(QRegExp("\\n"), " ");
        jsonObject.insert("FactorArgs",argsStr);
        doc = QJsonDocument(jsonObject);
        mmca.Args = doc.toJson();
    }

    bool r = this->bca.update(&mmca);
    //qDebug()<<"##############  initDataTables ......."<<endl;
    //创建数据表
    BHistoryData bhd;
    r=r&&bhd.initDataTables(mmca.PortName);
    //qDebug()<<__FUNCTION__<<": initDataTables result = "<<r<<endl;

    //系统端口参数刷新
    //mainWindow->reInitialize();

    if (r)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", mmca.PortName + " 参数保存成功！", QMessageBox::Ok, nullptr);
        messageBox.exec();
 //       emit toReboot();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", mmca.PortName + " 参数保存失败！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }
    emit toReboot();
}

void DialogSetupCom::initComponents(QString comType, int port)
{
    QString portName = comType + "-"+QString::number(port);
    int i;

    for (i=0;i<this->bca.ComArgsListNew.count();i++)
    {
        if ( this->bca.ComArgsListNew.at(i)->PortName == portName )
            break;
    }
    if (i>=this->bca.ComArgsListNew.count()) return;

    this->mcaPtr = this->bca.ComArgsListNew.at(i);

    this->mmca = *(this->mcaPtr);

    if (mmca.Status!=UNUSED)
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Checked);
        //初始化各控件数据

        ui->cmbSpeed->setCurrentText(QString::number(mmca.Speed)); //波特率
        if (mmca.DataBit == 7)
            ui->rbDataBit7->setChecked(true); //数据位
        else
            ui->rbDataBit8->setChecked(true);

        if (mmca.StopBit==1)
            ui->rbStopBit1->setChecked(true); //停止位
        else
            ui->rbStopBit2->setChecked(true);

        if (mmca.CheckBit == CHECK_EVEN)
            ui->rbCheckBitEven->setChecked(true); //校验
        else if (mmca.CheckBit == CHECK_ODD)
            ui->rbCheckBitOdd->setChecked(true); //校验
        else
            ui->rbCheckBitNone->setChecked(true);

        //设置监测因子的显示值
        QString factors="";
        for (int i=0;i<mmca.ComFactorArgsList.count();i++)
        {
            factors += mmca.ComFactorArgsList.at(i).chName +";";
        }
        factors.chop(1);
        ui->factors->setText(factors);
    }
    else
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Unchecked);
        //初始化各控件数据
        ui->cmbSpeed->setCurrentText("115200"); //默认波特率
        ui->rbDataBit8->setChecked(true); //默认数据位 8 位
        ui->rbStopBit1->setChecked(true); //默认停止位 1 位
        ui->rbCheckBitEven->setChecked(true); //默认偶校验
        ui->factors->setText(""); //监测因子为空
        mmca.ComFactorArgsList.clear();
    }

}


void DialogSetupCom::on_rbPort5_clicked()
{
    initComponents(this->ComType,5);
}

void DialogSetupCom::on_rbPort4_clicked()
{
    initComponents(this->ComType,4);
}

void DialogSetupCom::on_rbPort3_clicked()
{
    initComponents(this->ComType,3);
}

void DialogSetupCom::on_rbPort2_clicked()
{
    initComponents(this->ComType,2);
}

void DialogSetupCom::on_rbPort1_clicked()
{
    initComponents(this->ComType,1);
}

void DialogSetupCom::on_checkBoxPortEnabled_stateChanged(int arg1)
{
    if (mmca.Status!=UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Unchecked)
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
    else if (mmca.Status!=UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Checked)
    {
         setParamentComponentsEnable(true);
         ui->checkBoxPortEnabled->setText("已启用");
    }
    else if (mmca.Status==UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Unchecked)
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

void DialogSetupCom::setParamentComponentsEnable(bool enable)
{
    QString style, cmbcolor;
    ui->frameParaments->setEnabled(enable);
    if (enable==false)
    {
        ui->factors->setText("");
        ui->rbDataBit8->setChecked(true);
        ui->rbStopBit1->setChecked(true);
        ui->rbCheckBitEven->setChecked(true);
        ui->frameParaments->setStyleSheet("background-color: transparent;color:gray;");
        cmbcolor="gray";
    }
    else
    {
        ui->frameParaments->setStyleSheet("background-color: transparent;color:white;");
        cmbcolor="balck";
    }

    style = "QComboBox{";
    style += "background: rgba(0,0,0,0);";
    style += "background-color: transparent;";
    style += "color:"+cmbcolor+";";
    style += "border: 0px solid gray;";
    //style += "border-radius:3px;";
    style += "padding: 1px 2px 1px 2px;";
    style += "min-width:40em;";
    style += "selection-color:white;";
    style += "selection-background-color: #005aa7; }";
    style += "QComboBox::drop-down{";
    style += "background-color: transparent;";
    style += "subcontrol-origin: padding;";
    style += "subcontrol-position:top right;";
    style += "width:60px;";
    style += "border-left-width:0px;}";
   // style += "border-top-right-radius:3px;";
   // style += "border-bottom-right-radius:3px;}";
    style += "QComboBox  QAbstractItemView{";
    style += "background-color: #CCFFFF;}";
   // style += "border-radius:3px;  }";
    style += "QComboBox  QAbstractItemView::item{ height:30px; }";
    ui->cmbSpeed->setStyleSheet( style );
}

//打开维护监测因子对话框
void DialogSetupCom::on_btnBrowse_clicked()
{
    QDialog * dialog = new DialogSetupCommFactor(&mmca);
    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->show();
    connect(dialog, SIGNAL(updateFactors(QString)), this, SLOT(updateFactors(QString)));
}

void DialogSetupCom::updateFactors(QString factors)
{
    ui->factors->setText(factors);
}
