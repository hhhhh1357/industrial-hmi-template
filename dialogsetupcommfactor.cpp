#include "dialogsetupcommfactor.h"
#include "ui_dialogsetupcommfactor.h"

#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QException>
#include <QHeaderView>
#include <QList>
#include <QListWidgetItem>

#include "Headers/BLL/BFactor.h"

#include "mainwindow.h"

extern MainWindow * mainWindow;;

DialogSetupCommFactor::DialogSetupCommFactor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupCommFactor)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");

    ui->widget->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    ui->tableDetail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->widget->setFixedSize(200,60);
    ui->cmbDataType->setFixedSize(200,60);
}

DialogSetupCommFactor::DialogSetupCommFactor(MComArgs * comArgs,QWidget *parent):DialogSetupCommFactor(parent)
{
    this->comArgs = comArgs;
    ui->comName->setText(comArgs->PortName);    

    if (this->comArgs->PortName.indexOf("RS232")>=0 || this->comArgs->PortName.indexOf("RS485")>=0)
    {
        ui->stationAddr->installEventFilter(this);
        ui->registerAddr->installEventFilter(this);
        ui->cmdCode->installEventFilter(this);
        ui->valueCount->installEventFilter(this);
        initControlsRS232_485();
    }
    else
    {
        ui->stationAddr->setVisible(false);
        ui->registerAddr->setVisible(false);
        ui->cmdCode->setVisible(false);
        ui->valueCount->setVisible(false);
        ui->cmbDataType->setVisible(false);
        ui->lbStationAddr->setVisible(false);
        ui->lbRegisterAddr->setVisible(false);
        ui->lbCmdCode->setVisible(false);
        ui->lbValueCount->setVisible(false);
        ui->lbDataType->setVisible(false);

        ui->tableDetail->removeColumn(8);
        ui->tableDetail->removeColumn(5);
        ui->tableDetail->removeColumn(4);
        ui->tableDetail->removeColumn(3);
        ui->tableDetail->removeColumn(2);
        initControlsOther();
    }

    ui->factorC->installEventFilter(this);
    ui->alertMax->installEventFilter(this);
    ui->alertMin->installEventFilter(this);

    keyBoard = new KeyBoard(this);
}


DialogSetupCommFactor::~DialogSetupCommFactor()
{
    delete ui;
    delete keyBoard;
}

void DialogSetupCommFactor::initControlsRS232_485()
{
    for (int i=0;i<this->comArgs->ComFactorArgsList.count();i++)
    {
//        if (this->comArgs->ComFactorArgsList.at(i).flag>1)
//            continue;
        int iRow = ui->tableDetail->rowCount();
        ui->tableDetail->setRowCount(iRow + 1);//总行数增加1

        BFactor bf;

        QString nType = bf.getDataTypeByCode(this->comArgs->ComFactorArgsList.at(i).code);

        int p = nType.indexOf('.');
        int dn=0; //小数位数
        if (p>-1)
        {
            dn=nType.right(nType.length()-p-1).toInt();
        }

        ui->tableDetail->setItem(iRow, 0, new QTableWidgetItem( this->comArgs->ComFactorArgsList.at(i).code));
        ui->tableDetail->setItem(iRow, 1, new QTableWidgetItem(this->comArgs->ComFactorArgsList.at(i).chName));
        ui->tableDetail->setItem(iRow, 2, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).stationAddr)));
        ui->tableDetail->setItem(iRow, 3, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).registerAddr)));
        ui->tableDetail->setItem(iRow, 4, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).cmdCode)));
        ui->tableDetail->setItem(iRow, 5, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).valueCount)));
        ui->tableDetail->setItem(iRow, 6, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).warningValueMax,'f',dn)));
        ui->tableDetail->setItem(iRow, 7, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).warningValueMin,'f',dn)));
        ui->tableDetail->setItem(iRow, 8, new QTableWidgetItem(this->comArgs->ComFactorArgsList.at(i).plcDataType));
        ui->tableDetail->setItem(iRow, 9, new QTableWidgetItem(this->comArgs->ComFactorArgsList.at(i).deviceId));

        for (int i=0;i<10;i++)
           ui->tableDetail->item(iRow,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //水平和垂直居中
    }
    int iRowCount = ui->tableDetail->rowCount();
    if(iRowCount > 0)
    {
        ui->tableDetail->selectRow(0);
    }
}

void DialogSetupCommFactor::initControlsOther()
{

    for (int i=0;i<this->comArgs->ComFactorArgsList.count();i++)
    {
        int iRow = ui->tableDetail->rowCount();

        if (this->comArgs->ComFactorArgsList.at(i).flag>1)
            continue;

        ui->tableDetail->setRowCount(iRow + 1);//总行数增加1

        BFactor bf;
        //QString nType = bf.getDataTypeByCode(this->comArgs->ComFactorArgsList.at(i).factorCode);
        QString nType = bf.getDataTypeByCode(this->comArgs->ComFactorArgsList.at(i).code);

        int p = nType.indexOf('.');
        int dn=0; //小数位数
        if (p>-1)
        {
            dn=nType.right(nType.length()-p-1).toInt();
        }

        ui->tableDetail->setItem(iRow, 0, new QTableWidgetItem( this->comArgs->ComFactorArgsList.at(i).code));
        ui->tableDetail->setItem(iRow, 1, new QTableWidgetItem(this->comArgs->ComFactorArgsList.at(i).chName));
        ui->tableDetail->setItem(iRow, 2, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).warningValueMax,'f',dn)));
        ui->tableDetail->setItem(iRow, 3, new QTableWidgetItem(QString::number(this->comArgs->ComFactorArgsList.at(i).warningValueMin,'f',dn)));

        for (int i=0;i<4;i++)
           ui->tableDetail->item(iRow,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //水平和垂直居中
    }
    int iRowCount = ui->tableDetail->rowCount();
    if(iRowCount > 0)
    {
        ui->tableDetail->selectRow(0);
    }
}


void DialogSetupCommFactor::on_btnClose_clicked()
{
    close();
}

//RS232、RS485 : 根据输入的监测因子编号，查询因子相关国标信息和配置信息，并初始化相关控件
void DialogSetupCommFactor::setControlsValueRS232_485(const QString &factorCode)
{
    BFactor bf;
    MFactor factor;

    bool r = bf.getFactorInfoByCode(factorCode,factor);
    if (r) //找到了指定编码的监测因子
    {
        ui->factorName->setText(factor.chName);
        if (factor.cmdCode==0)
            ui->cmdCode->setText("3");
        else
            ui->cmdCode->setText(QString::number(factor.cmdCode));
        if (factor.valueCount==0)
            ui->valueCount->setText("2");
        else
            ui->valueCount->setText(QString::number(factor.valueCount));

        ui->stationAddr->setText(QString::number(factor.stationAddr));
        ui->registerAddr->setText(QString::number(factor.registerAddr));
        ui->alertMax->setText(QString::number(factor.warningValueMax,'f',6));
        ui->alertMin->setText(QString::number(factor.warningValueMin,'f',6));
        ui->cmbDataType->setEditText(factor.plcDataType);
        ui->deviceID->setText(factor.deviceId);

        int p = factor.nType.indexOf('.');
        int inu=1,dnu=0; //小数位数
        if (p>-1)
        {
            inu=factor.nType.left(p).right(p-1).toInt();
            dnu=factor.nType.right(factor.nType.length()-p-1).toInt();
        }
        else
        {
            inu=factor.nType.right(factor.nType.length()-1).toInt();
        }
        QString inputMask = QString("%1").arg(0,inu,10,QLatin1Char('0'));
        if (dnu>0)
        {
            inputMask += "."+QString("%1").arg(0,dnu,10,QLatin1Char('0'));
        }
        ui->alertMax->setInputMask(inputMask);
        ui->alertMin->setInputMask(inputMask);
    }
    else
    {
        ui->factorName->setText("");
        ui->cmdCode->setText("3");
        ui->valueCount->setText("2");
        ui->stationAddr->setText("");
        ui->registerAddr->setText("");
        ui->alertMax->setValidator(0);
        ui->alertMin->setValidator(0);
        ui->deviceID->setText("");
    }
}

//非RS232、RS485 : 根据输入的监测因子编号，查询因子相关国标信息和配置信息，并初始化相关控件
void DialogSetupCommFactor::setControlsValueOther(const QString &factorCode)
{
    BFactor bf;
    MFactor factor;

    bool r = bf.getFactorInfoByCode(factorCode,factor);
    if (r) //找到了指定编码的监测因子
    {
        ui->factorName->setText(factor.chName);
        ui->alertMax->setText(QString::number(factor.warningValueMax,'f',6));
        ui->alertMin->setText(QString::number(factor.warningValueMin,'f',6));;

        int p = factor.nType.indexOf('.');
        int inu=1,dnu=0; //小数位数
        if (p>-1)
        {
            inu=factor.nType.left(p).right(p-1).toInt();
            dnu=factor.nType.right(factor.nType.length()-p-1).toInt();
        }
        else
        {
            inu=factor.nType.right(factor.nType.length()-1).toInt();
        }
        QString inputMask = QString("%1").arg(0,inu,10,QLatin1Char('0'));
        if (dnu>0)
        {
            inputMask += "."+QString("%1").arg(0,dnu,10,QLatin1Char('0'));
        }
        ui->alertMax->setInputMask(inputMask);
        ui->alertMin->setInputMask(inputMask);
    }
    else
    {
        ui->factorName->setText("");
        ui->alertMax->setValidator(0);
        ui->alertMin->setValidator(0);
        ui->deviceID->setText("");
    }
}


void DialogSetupCommFactor::on_factorC_textEdited(const QString &arg1)
{
     if (this->comArgs->PortName.indexOf("RS232")>=0 || this->comArgs->PortName.indexOf("RS485")>=0)
     {
         QString factor = arg1;
         if (arg1.left(1)=="e"||arg1.left(1)=="g")
         {
             if (arg1.length() == 4)
             {
                 factor = factor + "xx";
             }

         }
         setControlsValueRS232_485(factor);
     }
     else
     {
         setControlsValueOther(arg1);
     }

}

int DialogSetupCommFactor::getItemRowByCode(QString code)
{
    for (int i=0;i<ui->tableDetail->rowCount();i++)
    {
        if (ui->tableDetail->item(i,0)->text() == code)
            return i;
    }
    return -1;
}

//增加
void DialogSetupCommFactor::on_btnAdd_clicked()
{
    if (ui->factorName->text().length()>0 && ui->registerAddr->text().trimmed().length()>0)
    {

        if (ui->alertMax->text().trimmed().toDouble()<=ui->alertMin->text().trimmed().toDouble())
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "告警值最大值要大于告警值最小值！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
            return;
        }

       int r = getItemRowByCode(ui->factorC->text());
       if (r!=-1) return ;

       int iRow = ui->tableDetail->rowCount();
       ui->tableDetail->setRowCount(iRow + 1);//总行数增加1

       if (this->comArgs->PortName.indexOf("RS232")>=0 || this->comArgs->PortName.indexOf("RS485")>=0)
       {
           ui->tableDetail->setItem(iRow, 0, new QTableWidgetItem(ui->factorC->text()));
           ui->tableDetail->setItem(iRow, 1, new QTableWidgetItem(ui->factorName->text()));
           ui->tableDetail->setItem(iRow, 2, new QTableWidgetItem(ui->stationAddr->text().trimmed()));
           ui->tableDetail->setItem(iRow, 3, new QTableWidgetItem(ui->registerAddr->text().trimmed()));
           ui->tableDetail->setItem(iRow, 4, new QTableWidgetItem(ui->cmdCode->text().trimmed()));
           ui->tableDetail->setItem(iRow, 5, new QTableWidgetItem(ui->valueCount->text().trimmed()));
           ui->tableDetail->setItem(iRow, 6, new QTableWidgetItem(ui->alertMax->text().trimmed()));
           ui->tableDetail->setItem(iRow, 7, new QTableWidgetItem(ui->alertMin->text().trimmed()));
           ui->tableDetail->setItem(iRow, 8, new QTableWidgetItem(ui->cmbDataType->currentText()));
           ui->tableDetail->setItem(iRow, 9, new QTableWidgetItem(ui->deviceID->text().trimmed()));
           for (int i=0;i<10;i++)
              ui->tableDetail->item(iRow,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //水平和垂直居中
       }
       else
       {
           ui->tableDetail->setItem(iRow, 0, new QTableWidgetItem(ui->factorC->text()));
           ui->tableDetail->setItem(iRow, 1, new QTableWidgetItem(ui->factorName->text()));
           ui->tableDetail->setItem(iRow, 2, new QTableWidgetItem(ui->alertMax->text().trimmed()));
           ui->tableDetail->setItem(iRow, 3, new QTableWidgetItem(ui->alertMin->text().trimmed()));
           ui->tableDetail->setItem(iRow, 4, new QTableWidgetItem(ui->deviceID->text().trimmed()));

           for (int i=0;i<5;i++)
              ui->tableDetail->item(iRow,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //水平和垂直居中
       }
   }
}

//删除表格项
void DialogSetupCommFactor::on_btnDelete_clicked()
{
    int rowIndex = ui->tableDetail->currentRow();
    if (rowIndex != -1)
        ui->tableDetail->removeRow(rowIndex);
}

//选中表格项
void DialogSetupCommFactor::on_tableDetail_itemSelectionChanged()
{
    int rowIndex = ui->tableDetail->currentRow();
    if (rowIndex != -1)
    {
        if (this->comArgs->PortName.indexOf("RS232")>=0 || this->comArgs->PortName.indexOf("RS485")>=0)
        {
            ui->factorC->setText(ui->tableDetail->item(rowIndex,0)->text());
            setControlsValueRS232_485(ui->tableDetail->item(rowIndex,0)->text());
            ui->factorName->setText(ui->tableDetail->item(rowIndex,1)->text());
            ui->stationAddr->setText(ui->tableDetail->item(rowIndex,2)->text());
            ui->registerAddr->setText(ui->tableDetail->item(rowIndex,3)->text());
            ui->cmdCode->setText(ui->tableDetail->item(rowIndex,4)->text());
            ui->valueCount->setText(ui->tableDetail->item(rowIndex,5)->text());
            ui->alertMax->setText(ui->tableDetail->item(rowIndex,6)->text());
            ui->alertMin->setText(ui->tableDetail->item(rowIndex,7)->text());
            ui->cmbDataType->setCurrentText(ui->tableDetail->item(rowIndex,8)->text());
            ui->deviceID->setText(ui->tableDetail->item(rowIndex,9)->text());
        }
        else
        {
            ui->factorC->setText(ui->tableDetail->item(rowIndex,0)->text());
            setControlsValueOther(ui->tableDetail->item(rowIndex,0)->text());
            ui->factorName->setText(ui->tableDetail->item(rowIndex,1)->text());
            ui->alertMax->setText(ui->tableDetail->item(rowIndex,2)->text());
            ui->alertMin->setText(ui->tableDetail->item(rowIndex,3)->text());
        }
    }
    else
    {
        if (this->comArgs->PortName.indexOf("RS232")>=0 || this->comArgs->PortName.indexOf("RS485")>=0)
        {
            ui->factorC->setText("");
            ui->factorName->setText("");
            ui->stationAddr->setText("");
            ui->registerAddr->setText("");
            ui->cmdCode->setText("3");
            ui->valueCount->setText("2");
            ui->alertMax->setText("");
            ui->alertMin->setText("");
            ui->deviceID->setText("");
        }
        else
        {
            ui->factorC->setText("");
            ui->factorName->setText("");
            ui->alertMax->setText("");MainWindow w;
            ui->alertMin->setText("");
        }
    }
}

//修改
void DialogSetupCommFactor::on_btnUpdate_clicked()
{
    if (ui->factorName->text().length()>0 && ui->registerAddr->text().trimmed().length()>0)
    {
        if (ui->alertMax->text().trimmed().toDouble()<=ui->alertMin->text().trimmed().toDouble())
        {
            QMessageBox messageBox(QMessageBox::Warning, "警告", "告警值最大值要大于告警值最小值！", QMessageBox::Ok, nullptr);
            messageBox.exec();
            return;
            return;
        }

       int iRow = getItemRowByCode(ui->factorC->text());
       if (iRow==-1) return ;
       if (this->comArgs->PortName.indexOf("RS232")>=0 || this->comArgs->PortName.indexOf("RS485")>=0)
       {
           ui->tableDetail->setItem(iRow, 0, new QTableWidgetItem(ui->factorC->text()));
           ui->tableDetail->setItem(iRow, 1, new QTableWidgetItem(ui->factorName->text()));
           ui->tableDetail->setItem(iRow, 2, new QTableWidgetItem(ui->stationAddr->text().trimmed()));
           ui->tableDetail->setItem(iRow, 3, new QTableWidgetItem(ui->registerAddr->text().trimmed()));
           ui->tableDetail->setItem(iRow, 4, new QTableWidgetItem(ui->cmdCode->text().trimmed()));
           ui->tableDetail->setItem(iRow, 5, new QTableWidgetItem(ui->valueCount->text().trimmed()));
           ui->tableDetail->setItem(iRow, 6, new QTableWidgetItem(ui->alertMax->text().trimmed()));
           ui->tableDetail->setItem(iRow, 7, new QTableWidgetItem(ui->alertMin->text().trimmed()));
           ui->tableDetail->setItem(iRow, 8, new QTableWidgetItem(ui->cmbDataType->currentText()));
           ui->tableDetail->setItem(iRow, 9, new QTableWidgetItem(ui->deviceID->text().trimmed()));
           for (int i=0;i<10;i++)
              ui->tableDetail->item(iRow,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //水平和垂直居中
       }
       else
       {
           ui->tableDetail->setItem(iRow, 0, new QTableWidgetItem(ui->factorC->text()));
           ui->tableDetail->setItem(iRow, 1, new QTableWidgetItem(ui->factorName->text()));
           ui->tableDetail->setItem(iRow, 2, new QTableWidgetItem(ui->alertMax->text().trimmed()));
           ui->tableDetail->setItem(iRow, 3, new QTableWidgetItem(ui->alertMin->text().trimmed()));

           for (int i=0;i<4;i++)
              ui->tableDetail->item(iRow,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //水平和垂直居中
       }
   }
}

//保存并且退出
void DialogSetupCommFactor::on_btnSave_clicked()
{
     QMutexLocker locker(&(BComArgs::mutexComArgsList));
     QMutexLocker locker1(&(mainWindow->mutexDataItemList));
     QMutexLocker locker2(&(mainWindow->mutexDataComArgsList));

     QString factors="";
     this->comArgs->ComFactorArgsList.clear();
//     for (int i=this->comArgs->ComFactorArgsList.count()-1;i>=0;i--)
//     {
//         this->comArgs->ComFactorArgsList.removeAt(i);
//     }


     QJsonArray jsonArray;
     //更新列表
     int iRow = ui->tableDetail->rowCount();
     for (int i=0;i<iRow;i++)
     {         
         BFactor bf;
         MFactor cfa;
         cfa.code = ui->tableDetail->item(i,0)->text();
         bf.getFactorInfoByCode(cfa.code,cfa);

         cfa.dataPortName = this->comArgs->PortName;
         if (this->comArgs->PortName.indexOf("RS232")>=0 || this->comArgs->PortName.indexOf("RS485")>=0)
         {
             cfa.stationAddr = ui->tableDetail->item(i,2)->text().toUInt();
             cfa.registerAddr = ui->tableDetail->item(i,3)->text().toUInt();
             cfa.cmdCode = ui->tableDetail->item(i,4)->text().toInt();
             cfa.valueCount =ui->tableDetail->item(i,5)->text().toInt();
             cfa.warningValueMax = ui->tableDetail->item(i,6)->text().toDouble();
             cfa.warningValueMin = ui->tableDetail->item(i,7)->text().toDouble();
             cfa.plcDataType = ui->tableDetail->item(i,8)->text();
             cfa.deviceId = ui->tableDetail->item(i,9)->text();
         }
         else
         {
             cfa.warningValueMax = ui->tableDetail->item(i,2)->text().toDouble();
             cfa.warningValueMin = ui->tableDetail->item(i,3)->text().toDouble();
         }

         if (cfa.flag==0)
         {
             //生成数据表的名称
             if (cfa.deviceId.trimmed().length()>0)
             {
                 cfa.localTableName = cfa.dataPortName+"_"+cfa.deviceId.trimmed()+"_"+cfa.code;
                 cfa.localTableName = cfa.localTableName.replace("xx",cfa.deviceId.trimmed());
             }
             else
             {
                 cfa.localTableName=cfa.dataPortName+"_"+cfa.code;
             }
         }

         bf.saveFactor(cfa);
         this->comArgs->ComFactorArgsList.append(cfa);

         //拼接因子中文名称，到上一界面显示
         factors += cfa.chName +";";
     }
     factors.chop(1);

     emit updateFactors(factors);

     close();
}


bool DialogSetupCommFactor::eventFilter(QObject *obj, QEvent*event)
{
    if (event->type()==QEvent::FocusIn)
    {
        keyBoard->show();
        ((QLineEdit*)obj)->setFocus();
        return true;
    }
    else
    {
        return QWidget::eventFilter(obj,event);
    }
}
