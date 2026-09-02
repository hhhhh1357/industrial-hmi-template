#include "dialogsetupai.h"
#include "ui_dialogsetupai.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <Headers/BLL/BHistoryData.h>

extern MainWindow * mainWindow;

DialogSetupAI::DialogSetupAI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupAI)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
    this->bca.queryDataComArgsList(&(this->bca.ComArgsListNew));
}

DialogSetupAI::DialogSetupAI(QString comType,QWidget *parent):DialogSetupAI(parent)
{
    this->AiType = comType ;
    //设置只能输入数字和小数点
    QRegExp rx("[0-9.0-9]*");
    pReg = new QRegExpValidator(rx,this);
    ui->dataRangeMin->setValidator(pReg);
    ui->dataRangeMax->setValidator(pReg);
    ui->factorDataRangeMin->setValidator(pReg);
    ui->factorDataRangeMax->setValidator(pReg);
    ui->alertMax->setValidator(pReg);
    ui->alertMin->setValidator(pReg);

    initComponents(this->AiType,1); //显示端口1
    keyBoard = new KeyBoard(this);
    ui->rbPort1->setFocus();
}


DialogSetupAI::~DialogSetupAI()
{    
    delete ui;
    delete pReg;
    if (keyBoard!=nullptr)
       delete keyBoard;
}

void DialogSetupAI::on_btnClose_clicked()//关闭窗口
{
    close();
}

void DialogSetupAI::on_btnSave_clicked()
{
    if (ui->factorC->text().trimmed().length()==0 && ui->checkBoxPortEnabled->checkState()==Qt::CheckState::Checked)
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "未配置监测因子！", QMessageBox::Ok, nullptr);
        messageBox.exec();
        return;
    }

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
        MFactor factor;
        BFactor bf;
        bool r = bf.getFactorInfoByCode(ui->factorC->text(),factor);
        if (r==false)
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "编号为: "+ui->factorC->text()+ " 的监测因子不存在！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
        }

        if (ui->rbSignalTypeV->isChecked())
        {
            mca.SignalType = VOLTAGE;  //信号类型：电压
        }
        else
        {
            mca.SignalType = CURRENT;  //信号类型：电流ui->checkBoxPortEnabled->checkState()
        }

        mca.DataRangeMax = ui->dataRangeMax->text().toDouble();
        mca.DataRangeMin = ui->dataRangeMin->text().toDouble();

        mca.FactorDataRangeMax = ui->factorDataRangeMax->text().toDouble();
        mca.FactorDataRangeMin = ui->factorDataRangeMin->text().toDouble();

        QJsonObject jsonObject;
        jsonObject.insert("SignalType",mca.SignalType);
        jsonObject.insert("DataRangeMax",mca.DataRangeMax);
        jsonObject.insert("DataRangeMin",mca.DataRangeMin);
        jsonObject.insert("FactorDataRangeMax",mca.FactorDataRangeMax);
        jsonObject.insert("FactorDataRangeMin",mca.FactorDataRangeMin);

        if (mca.DataRangeMax<=mca.DataRangeMin)
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "量程最大值要大于量程最小值！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
        }
        if (mca.FactorDataRangeMax<=mca.FactorDataRangeMin)
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "因子值最大值要大于因子值最小值！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
        }


        mca.ComFactorArgsList.clear();
        factor.dataPortName = portName;

        factor.warningValueMax = ui->alertMax->text().toDouble();
        factor.warningValueMin = ui->alertMin->text().toDouble();
        factor.deviceId = ui->deviceId->text();
        factor.signalType = mca.SignalType; //监测因子 监测时的信号类型在

        if (factor.warningValueMax<=factor.warningValueMin)
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "告警值最大值要大于告警值最小值！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
        }

        if (factor.warningValueMax>=mca.FactorDataRangeMax)
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "告警值最大值要小于因子值的最大值！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
        }
        if (factor.warningValueMin<=mca.FactorDataRangeMin)
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "告警值最小值要大于因子值的最小值！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
        }

        factor.maxValue = mca.FactorDataRangeMax;
        factor.minValue = mca.FactorDataRangeMin;

        //生成数据表的名称
        if (factor.deviceId.trimmed().length()>0)
        {
            factor.localTableName = factor.dataPortName+"_"+factor.deviceId.trimmed()+"_"+factor.code;
            factor.localTableName = factor.localTableName.replace("xx",factor.deviceId.trimmed());
        }
        else
        {
            factor.localTableName=factor.dataPortName+"_"+factor.code;
        }

        bf.saveFactor(factor);
        mca.ComFactorArgsList.append(factor);

        QJsonArray jsonArray;
        for (auto & cfa : mca.ComFactorArgsList)
             jsonArray.append(cfa.argsToJson());
        auto doc = QJsonDocument(jsonArray);
        QString argsStr = doc.toJson();
        argsStr = argsStr.replace(QRegExp("\\n"), " ");
        jsonObject.insert("FactorArgs",argsStr);
        doc = QJsonDocument(jsonObject);
        mca.Args = doc.toJson();
    }
    BComArgs bca;
    bool rr = bca.update(&(this->mca));

    //创建数据表
    BHistoryData bhd;
    rr = bhd.initDataTables(mca.PortName);

    //重新初始化数据
    //mainWindow->reInitialize();

    if (rr)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", this->mca.PortName + " 参数保存成功！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "警告", this->mca.PortName + " 参数保存失败！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }

    emit toReboot();

}

void DialogSetupAI::on_rbPort8_clicked()
{
    initComponents(this->AiType,8);
}

void DialogSetupAI::on_rbPort7_clicked()
{
    initComponents(this->AiType,7);
}

void DialogSetupAI::on_rbPort6_clicked()
{
    initComponents(this->AiType,6);
}

void DialogSetupAI::on_rbPort5_clicked()
{
     initComponents(this->AiType,5);
}

void DialogSetupAI::on_rbPort4_clicked()
{
     initComponents(this->AiType,4);
}

void DialogSetupAI::on_rbPort3_clicked()
{
     initComponents(this->AiType,3);
}

void DialogSetupAI::on_rbPort2_clicked()
{
     initComponents(this->AiType,2);
}

void DialogSetupAI::on_rbPort1_clicked()
{
    initComponents(this->AiType,1);
}

void DialogSetupAI::on_checkBoxPortEnabled_stateChanged(int arg1)
{

    int i;
    for (i=0;i<this->bca.ComArgsListNew.count();i++)
    {
        if ( this->bca.ComArgsListNew.at(i)->PortName == mca.PortName )
            break;
    }
    if (i>=this->bca.ComArgsListNew.count()) return;


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
               setComponentsEnable(false);
               ui->checkBoxPortEnabled->setText("未启用");
               this->bca.ComArgsListNew.at(i)->Status = UNUSED;
           }
       }
       else if (mca.Status!=UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Checked)
       {
            if (ui->rbSignalTypeV->isChecked())
                on_rbSignalTypeV_clicked();
            else
                on_rbSignalTypeA_clicked();

            setComponentsEnable(true);
            ui->checkBoxPortEnabled->setText("已启用");

            this->bca.ComArgsListNew.at(i)->Status = NORMAL;
       }
       else if (mca.Status==UNUSED && ui->checkBoxPortEnabled->checkState() == Qt::CheckState::Unchecked)
       {
           // 相关组件设置成 无效
           setComponentsEnable(false);
           ui->checkBoxPortEnabled->setText("未启用");

           this->bca.ComArgsListNew.at(i)->Status = UNUSED;
       }
       else
       {
           if (ui->rbSignalTypeV->isChecked())
             on_rbSignalTypeV_clicked();
           else
             on_rbSignalTypeA_clicked();

           setComponentsEnable(true);
           ui->checkBoxPortEnabled->setText("已启用");

           this->bca.ComArgsListNew.at(i)->Status = NORMAL;
       }
}

void DialogSetupAI::initComponents(QString AiType, int port)
{
    portName = AiType + "-"+QString::number(port);

    int i;
    for (i=0;i<this->bca.ComArgsListNew.count();i++)
    {
        if ( this->bca.ComArgsListNew.at(i)->PortName == portName )
            break;
    }
    if (i>=this->bca.ComArgsListNew.count()) return;

    mca = *(this->bca.ComArgsListNew.at(i));

    if (mca.Status!=UNUSED)
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Checked);
        if (mca.SignalType == VOLTAGE)
        {
            ui->rbSignalTypeV->setChecked(true);
            ui->lbUnitMax->setText("V");
            ui->lbUnitMin->setText("V");

        }
        else
        {
            ui->rbSignalTypeA->setChecked(true);
            ui->lbUnitMax->setText("mA");
            ui->lbUnitMin->setText("mA");
        }

        ui->dataRangeMax->setText(QString::number(mca.DataRangeMax));
        ui->dataRangeMin->setText(QString::number(mca.DataRangeMin));

        ui->factorDataRangeMax->setText(QString::number(mca.FactorDataRangeMax));
        ui->factorDataRangeMin->setText(QString::number(mca.FactorDataRangeMin));

        if (mca.ComFactorArgsList.count()>0)
        {
            MFactor factor = mca.ComFactorArgsList.at(0);
            ui->factorC->setText(factor.code);
            ui->factorName->setText(factor.chName);

            int inu=1,dnu=0; //小数位数
            getInuDnu(&factor,inu,dnu);

            ui->alertMax->setText(QString::number(factor.warningValueMax,10,dnu));
            ui->alertMin->setText(QString::number(factor.warningValueMin,10,dnu));
            ui->deviceId->setText(factor.deviceId);
        }
    }
    else
    {
        ui->checkBoxPortEnabled->setCheckState(Qt::CheckState::Unchecked);
        ui->rbSignalTypeV->setChecked(true);
        ui->dataRangeMax->setText("");
        ui->dataRangeMin->setText("");
        ui->factorDataRangeMax->setText("");
        ui->factorDataRangeMin->setText("");
        ui->factorC->setText("");
        ui->factorName->setText("");
        ui->alertMax->setText("");
        ui->alertMin->setText("");
        ui->deviceId->setText("");
    }

}

void DialogSetupAI::setComponentsEnable(bool enable)
{
   QString style, cmbcolor;
   ui->frameParaments->setEnabled(enable);
   if (enable==false)
   {
       //ui->dataRangeMax->setText("5");
       //ui->dataRangeMin->setText("1");
       //ui->lbUnitMax->setText("V");
       //ui->lbUnitMin->setText("V");

       ui->factorC->setText("");
       ui->factorName->setText("");
       ui->alertMax->setText("");
       ui->alertMin->setText("");
       ui->deviceId->setText("");

       this->mca.ComFactorArgsList.clear();
       ui->frameParaments->setStyleSheet("background-color: transparent;color:gray;");
       cmbcolor="gray";
   }
   else
   {
       ui->frameParaments->setStyleSheet("background-color: transparent;color:white;");
       cmbcolor="white";
   }

}

void DialogSetupAI::setFactorParamentValue(QString factorCode)
{
    BFactor bf;
    MFactor factor;

    bool r = bf.getFactorInfoByCode(factorCode,factor);
    if (r) //找到了指定编码的监测因子
    {
        ui->factorName->setText(factor.chName);

        int inu=1,dnu=0; //小数位数
        getInuDnu(&factor,inu,dnu);

        ui->alertMax->setText(QString::number(factor.warningValueMax,'f',dnu));
        ui->alertMin->setText(QString::number(factor.warningValueMin,'f',dnu));
        ui->deviceId->setText(factor.deviceId);
    }
    else
    {
        ui->factorName->setText("");
        ui->alertMax->setText("");
        ui->alertMin->setText("");
        ui->deviceId->setText("");
    }
}

void DialogSetupAI::getInuDnu(MFactor *factor, int &inu, int &dnu)
{
    if (factor==nullptr)
    {
        inu=0;
        dnu=0;
        return ;
    }
    int p = factor->nType.indexOf('.');
    if (p>-1)
    {
        inu=factor->nType.left(p).right(p-1).toInt();
        dnu=factor->nType.right(factor->nType.length()-p-1).toInt();
    }
    else
    {
        inu=factor->nType.right(factor->nType.length()-1).toInt();
        dnu=0;
    }
}

void DialogSetupAI::on_rbSignalTypeV_clicked()
{
    ui->lbUnitMax->setText("V");
    ui->lbUnitMin->setText("V");
    ui->dataRangeMax->setText("5");
    ui->dataRangeMin->setText("1");
}

void DialogSetupAI::on_rbSignalTypeA_clicked()
{
    ui->lbUnitMax->setText("mA");
    ui->lbUnitMin->setText("mA");
    ui->dataRangeMax->setText("20");
    ui->dataRangeMin->setText("4");
}

void DialogSetupAI::on_factorC_textEdited(const QString &arg1)
{
    setFactorParamentValue(arg1);
}
