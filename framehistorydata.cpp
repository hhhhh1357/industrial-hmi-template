#include "framehistorydata.h"
#include "mainwindow.h"
#include "ui_framehistorydata.h"

#include <Headers/BLL/BFactor.h>

#include <Sources/Tools/sysutil.h>

extern MainWindow *mainWindow;

FrameHistoryData::FrameHistoryData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameHistoryData)
{
    ui->setupUi(this);

    //填充监测因子下拉列表框
    ui->factor->clear();
    ui->factor->addItem("请选择监测因子");
    QList<MFactor *> factorList=mainWindow->getDataItemList();
    for (int i=0;i<factorList.count();i++)
    {
         if (factorList.at(i)->localTableName.length()>0)
         {
             MFactor * factor = factorList.at(i);
             if (factor->deviceId.length()>0)
                 ui->factor->addItem(factor->chName+"-"+factor->code+"@"+factor->dataPortName+":"+factor->deviceId);
             else
                 ui->factor->addItem(factor->chName+"-"+factor->code+"@"+factor->dataPortName);
         }
    }

    //默认时间
    QDateTime timeE = QDateTime::currentDateTime();
    ui->timeEnd->setDateTime(timeE);
    ui->timeStart->setDateTime(timeE.addSecs(-8*60*60));

    //表格属性设置
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background: rgb(255, 255, 127);}");


    //翻页按钮不可见
    ui->btnUp->setVisible(false);
    ui->btnDown->setVisible(false);
    ui->btnPageUp->setVisible(false);
    ui->btnPageDown->setVisible(false);
    ui->btnSaveSD->setVisible(false);
    ui->btnSaveUSB->setVisible(false);
    ui->tableView->setVisible(false);
    ui->frameDiagram->setVisible(false);
}

FrameHistoryData::~FrameHistoryData()
{
    delete ui;
    if (frameDiagram!=nullptr)
        delete frameDiagram;
}

void FrameHistoryData::on_btnQuery_clicked()
{
    if (ui->factor->currentIndex()==0)
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", "请先选择监测因子！", QMessageBox::Ok, NULL);
        messageBox.exec();
        return;
    }

    this->selectItem = ui->factor->currentText();
    this->factorName = selectItem.section("-",0,0);
    this->factorCode = selectItem.section("-",1,1).section("@",0,0);

    if (selectItem.indexOf(":")>0)
    {
        this->dataPortName = selectItem.section("@",1,1).section(":",0,0);
        this->dataDeviceId = selectItem.section("@",1,1).section(":",1,1);
    }
    else
    {
        this->dataPortName = selectItem.section("@",1,1);
    }

    BFactor bfactor;
    bfactor.getFactorInfoByCode(this->factorCode,this->factor);

    BHistoryData historyData;
    //查询数据
    if (ui->rbDataReal->isChecked())   //查询实时数据
    {
        this->recCount = historyData.getRecordCount(this->factor.localTableName, ui->timeStart->dateTime(),ui->timeEnd->dateTime());
    }
    else if (ui->rbDataMinute->isChecked()) //查询分钟数据
    {
        this->recCount = historyData.getRecordCountM(this->factor.code, ui->timeStart->dateTime(),ui->timeEnd->dateTime());
        this->recStart=0;
    }
    else if (ui->rbDataHour->isChecked())  //查询小时数据
    {
        this->recCount = historyData.getRecordCountH(this->factor.code, ui->timeStart->dateTime(),ui->timeEnd->dateTime());
        this->recStart=0;
    }
    else //查询日数据
    {
        this->recCount = historyData.getRecordCountD(this->factor.code, ui->timeStart->dateTime(),ui->timeEnd->dateTime());
        this->recStart=0;
    }

    if (recCount==0)
    {
        ui->btnSaveSD->setVisible(false);
        ui->btnSaveUSB->setVisible(false);
        ui->tableView->setVisible(false);
        ui->frameDiagram->setVisible(false);
        setPageButtonStatus();
        return;
    }

    this->recStart=0;
    if (this->recCount<this->recStart+this->recPerPage)
        this->recEnd = this->recCount;
    else
        this->recEnd=this->recStart+this->recPerPage;

    bool r = queryData();
    if (r==false) return ;

    ui->tableView->setVisible(true);
    ui->frameDiagram->setVisible(true);


    setPageButtonStatus();

    addDataToDiagram();

    fillTable();

    if (SysUtil::getUSBStatus())
        ui->btnSaveUSB->setVisible(true);
    else
        ui->btnSaveUSB->setVisible(false);

    if (SysUtil::getSDcardStatus())
        ui->btnSaveSD->setVisible(true);
    else
        ui->btnSaveSD->setVisible(false);
}

void FrameHistoryData::addDataToDiagram()
{
    if (frameDiagram==nullptr)
    {
        frameDiagram = new FrameDiagram(ui->frameDiagram);
        frameDiagram->setFixedSize(900,500);
        frameDiagram->show();
    }
    if (ui->rbDataReal->isChecked())   //查询实时数据
    {
       frameDiagram->setDiagramArgs(this->factorName, this->factor.unitC, this->values, this->timestamps,
                                  this->factor.maxValue, this->factor.minValue, this-> factor.warningValueMax,this->factor.warningValueMin);
    }
    else //查询的是分钟或小时数据
    {
        frameDiagram->setDiagramArgs(this->factorName, this->factor.unitC, this->valuesMax,
                                     this->valuesMin,  this->valuesAvg,this->timestamps);
    }

}

void FrameHistoryData::setPageButtonStatus()
{
   if (this->recCount==0)
   {
       ui->btnUp->setVisible(false);
       ui->btnDown->setVisible(false);
       ui->btnPageUp->setVisible(false);
       ui->btnPageDown->setVisible(false);
       return;
   }
   if (this->recStart>0)
   {
       ui->btnUp->setVisible(true);
       ui->btnPageUp->setVisible(true);
   }
   else
   {
       ui->btnUp->setVisible(false);
       ui->btnPageUp->setVisible(false);
   }

   if (this->recEnd==this->recCount)
   {
       ui->btnDown->setVisible(false);
       ui->btnPageDown->setVisible(false);
   }
   else
   {
       ui->btnDown->setVisible(true);
       ui->btnPageDown->setVisible(true);
   }

}

void FrameHistoryData::fillTable()
{
    if (model!=nullptr)
        delete model;

    if (ui->rbDataReal->isChecked())
    {
        model = new QStandardItemModel(this->values.count(),3) ;
        QStringList labels = QObject::trUtf8("序号,时间,值").simplified().split(",");
        model->setHorizontalHeaderLabels(labels);
        ui->tableView->setModel(model);
        QStandardItem * item = nullptr;
        for (int i=0;i<this->values.count();i++)
        {
            item = new QStandardItem(QString("%1").arg((i+1)));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i,0,item);

            item = new QStandardItem(QString("%1").arg((QDateTime::fromMSecsSinceEpoch(this->timestamps.at(i))).toString("MM-dd HH:mm:ss")));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i,1,item);

            item = new QStandardItem(QString("%1").arg(this->values.at(i)));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i,2,item);
        }
        ui->tableView->setColumnWidth(0,20);
        ui->tableView->setColumnWidth(1,160);
        ui->tableView->setColumnWidth(2,100);
    }
    else
    {
        model = new QStandardItemModel(this->timestamps.count(),4) ;
        QStringList labels = QObject::trUtf8("时间,最大值,最小值,平均值").simplified().split(",");
        model->setHorizontalHeaderLabels(labels);
        ui->tableView->setModel(model);
        QStandardItem * item = nullptr;
        for (int i=0;i<this->timestamps.count();i++)
        {
            if (ui->rbDataMinute->isChecked())
                item = new QStandardItem(QString("%1").arg((QDateTime::fromMSecsSinceEpoch(this->timestamps.at(i))).toString("MM-dd HH:mm")));
            else
                item = new QStandardItem(QString("%1").arg((QDateTime::fromMSecsSinceEpoch(this->timestamps.at(i))).toString("MM-dd HH")));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i,0,item);

            item = new QStandardItem(QString("%1").arg(this->valuesMax.at(i)));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i,1,item);

            item = new QStandardItem(QString("%1").arg(this->valuesMin.at(i)));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i,2,item);

            item = new QStandardItem(QString("%1").arg(this->valuesAvg.at(i)));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i,3,item);
        }
        ui->tableView->setColumnWidth(0,130);
        ui->tableView->setColumnWidth(1,65);
        ui->tableView->setColumnWidth(2,65);
        ui->tableView->setColumnWidth(3,65);
    }
}

void FrameHistoryData::exportData(QString driver)
{
    QString fileName="";

    if (driver=="USB")
    {
        fileName="/mnt/usb/";
    }
    else if (driver=="SD")
    {
        fileName="/mnt/sd/";
    }
    else
        return ;

    //fileName+="DAQ&T_Data.csv";
    fileName+="DAQ&T_Data_"+this->factor.code;
    if (ui->rbDataReal->isChecked())
        fileName +="_R";
    else if (ui->rbDataMinute->isChecked())
        fileName +="_M";
    else if (ui->rbDataHour->isChecked())
        fileName +="_H";
    else
        fileName +="_D";
    fileName +=".csv";

//    fileName += this->factor.code;  //因子编码
//    if (ui->rbDataReal->isChecked())
//        fileName +="-Real";
//    else if (ui->rbDataMinute->isChecked())
//        fileName +="-Minute";
//    else if (ui->rbDataHour->isChecked())
//        fileName +="-Hour";
//    else
//        fileName +="-Day";

//    fileName += "-"+ui->timeStart->dateTime().toString("yyyyMMddHHmmss");
//    fileName += "-"+ui->timeEnd->dateTime().toString("yyyyMMddHHmmss")+".csv";

    bool r ;
    if (ui->rbDataReal->isChecked())
    {
        QList<QString> colName;
        colName << "序号" << "时间" << "值" ;
        QList<QVector<double>*> data ;
        data.append(&(this->timestamps));
        data.append(&(this->values));
        //SysUtil::exportExcel(fileName, colName, data);
        r=SysUtil::exportCSV(fileName, colName, data);
    }
    else
    {
        QList<QString> colName;
        colName << "序号" << "时间" << "最大值" << "最小值"<< "平均值" ;
        QList<QVector<double>*> data ;
        data.append(&(this->timestamps));
        data.append(&(this->valuesMax));
        data.append(&(this->valuesMin));
        data.append(&(this->valuesAvg));
        //SysUtil::exportExcel(fileName, colName, data);
        r=SysUtil::exportCSV(fileName, colName, data);
    }

    if (r)
    {
        QMessageBox messageBox(QMessageBox::Information, "提示信息", "数据导出成功！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "数据导出失败！！！", QMessageBox::Ok, NULL);
        messageBox.exec();
    }
}

bool FrameHistoryData::queryData()
{
    BHistoryData historyData;
    bool r=false;
    if (ui->rbDataReal->isChecked())   //查询实时数据
    {
        this->values.clear();
        this->timestamps.clear();
        r = historyData.getSerialData(this->factor.localTableName, ui->timeStart->dateTime(), ui->timeEnd->dateTime(),
                      this->recPerPage, this->recStart, this->values, this->timestamps);
    }
    else if (ui->rbDataMinute->isChecked()) //查询分钟数据
    {
        this->valuesAvg.clear();
        this->valuesMax.clear();
        this->valuesMin.clear();
        this->timestamps.clear();
        r = historyData.getSerialDataM(this->factor.code, ui->timeStart->dateTime(), ui->timeEnd->dateTime(),
                      this->recPerPage, this->recStart, this->valuesMax, this->valuesMin, this->valuesAvg,this->timestamps);

    }
    else if (ui->rbDataHour->isChecked())//查询小时数据
    {
        this->valuesAvg.clear();
        this->valuesMax.clear();
        this->valuesMin.clear();
        this->timestamps.clear();
        r = historyData.getSerialDataH(this->factor.code, ui->timeStart->dateTime(), ui->timeEnd->dateTime(),
                      this->recPerPage, this->recStart, this->valuesMax, this->valuesMin, this->valuesAvg,this->timestamps);
    }
    else  //查询日数据
    {
        this->valuesAvg.clear();
        this->valuesMax.clear();
        this->valuesMin.clear();
        this->timestamps.clear();
        r = historyData.getSerialDataD(this->factor.code, ui->timeStart->dateTime(), ui->timeEnd->dateTime(),
                      this->recPerPage, this->recStart, this->valuesMax, this->valuesMin, this->valuesAvg,this->timestamps);
    }
    return r;
}

void FrameHistoryData::on_btnDown_clicked()
{
    if (this->recStart==0 && this->recEnd==0 || this->recStart+this->recPerPage/2>=this->recCount)
        return;

    recStart += recPerPage/2;
    if (recStart+recPerPage>recCount)
        recEnd=recCount;
    else
        recEnd+=recEnd/2;

    setPageButtonStatus();

    queryData();

    addDataToDiagram();

    fillTable();
}

void FrameHistoryData::on_btnUp_clicked()
{
    if (this->recStart==0)
        return;

    if (recStart-recPerPage/2<0)
    {
        recStart = 0;
    }
    else
    {
        recStart -= recPerPage/2;
    }
    recEnd=recStart+recPerPage;

    setPageButtonStatus();

    queryData();

    addDataToDiagram();

    fillTable();
}


void FrameHistoryData::on_btnPageDown_clicked()
{
    if (this->recStart==0 && this->recEnd==0 || this->recStart+this->recPerPage*3/4>=this->recCount)
        return;

    recStart += recPerPage*3/4;
    if (recStart+recPerPage>recCount)
        recEnd=recCount;
    else
        recEnd+=recEnd*3/4;

    setPageButtonStatus();

    queryData();

    addDataToDiagram();

    fillTable();
}

void FrameHistoryData::on_btnPageUp_clicked()
{
    if (this->recStart==0)
        return;

    if (recStart-recPerPage*3/4<0)
    {
        recStart = 0;
    }
    else
    {
        recStart -= recPerPage*3/4;
    }
    recEnd=recStart+recPerPage;

    setPageButtonStatus();

    queryData();

    addDataToDiagram();

    fillTable();
}

void FrameHistoryData::on_btnSaveSD_clicked()
{
    exportData("SD");
}

void FrameHistoryData::on_btnSaveUSB_clicked()
{
    exportData("USB");
}
