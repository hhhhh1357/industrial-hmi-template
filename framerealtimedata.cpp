#include "formlogin.h"
#include "framerealtimedata.h"
#include "ui_framerealtimedata.h"
#include <QDebug>

#include <Sources/Tools/datatype.h>

#include <QIcon>
extern FormLogin * loginWindow;
extern MainWindow * mainWindow;

FrameRealTimeData::FrameRealTimeData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameRealTimeData)
{
    ui->setupUi(this);
}

FrameRealTimeData::FrameRealTimeData(QList<MFactor *> * DataItemList, QList<MFactor *> * EnvinmentItemList, MFactor * wqs, QWidget *parent):FrameRealTimeData(parent)
{
    this->DataItemList = DataItemList;
    this->EnvironmentItemList = EnvinmentItemList;
    this->wqs=wqs;
    // 设置主窗口背景颜色

   // this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #64b3f4, stop:1 #c2e59c);");

    setComponent(); //设置组件

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
}


FrameRealTimeData::~FrameRealTimeData()
{
  //qDebug()<<__FILE__<<", " <<__FUNCTION__<<endl;

    delete ui;
    delete tabW;
    delete tabE;

    if (thermoList!=nullptr)
    {
        for (int i=0;i<thermoList->count();i++)
            delete thermoList->at(i);
        delete thermoList;
    }
}

void FrameRealTimeData::setComponent()
{
    //设置图组件
    //设置组件背景图
    //ui->lbUp->setPixmap(QPixmap(":/UP.bng"));
//    ui->lbUp->setPixmap(QPixmap(":/Images"));

//    ui->lbDown->setPixmap(QPixmap(":/DOWN.png"));


    //设置 翻页组件 透明
    //ui->lbUp->setStyleSheet("background-color: transparent;");
    //ui->lbDown->setStyleSheet("background-color: transparent;");
    //为翻页组件安装事件过滤器
    ui->lbUp->installEventFilter(this);
    ui->lbDown->installEventFilter(this);

    //图表的列表
    thermoList = new QList<FrameThermo*>();

    //计算图表的总个数和总页数
    thermoTotals = this->DataItemList->count();
    totalPages = thermoTotals/(colsPerPage*rowsPerPage);

    if (thermoTotals%(colsPerPage*rowsPerPage)!=0)
       totalPages ++;
    currPage = 1;

    //设置表
    //为翻页组件安装事件过滤器
//    ui->lbUpT->installEventFilter(this);
//    ui->lbDownT->installEventFilter(this);
    totalItemsTable = this->EnvironmentItemList->count();
    totalPagesTable = totalItemsTable/rowsPerPageTable+1;
    currPageTable = 1;

    tabW=new QLabel();
    tabW->setText("水\n质\n采\n样\n器");
    QString style="QLabel{background-color: transparent;color:white;font:22px;width:60px}";
    tabW->setStyleSheet(style);
    ui->tabWidget->tabBar()->setTabButton(0,QTabBar::ButtonPosition::LeftSide,tabW);

    tabE=new QLabel();
    tabE->setText("工\n作\n环\n境");
    style="QLabel{background-color: transparent;color:gray;font:22px;width:60px}";
    tabE->setStyleSheet(style);
    ui->tabWidget->tabBar()->setTabButton(1,QTabBar::ButtonPosition::LeftSide,tabE);

    for (int i=0;i<this->EnvironmentItemList->count();i++)
    {
        envStatueItem[i][0].setText(this->EnvironmentItemList->at(i)->chName);
        ui->tableDetail->setItem(i, 0, &envStatueItem[i][0]);
        ui->tableDetail->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        envStatueItem[i][1].setText("无数据");
        ui->tableDetail->setItem(i, 1, &envStatueItem[i][1]);
        ui->tableDetail->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    statueItem[0][0].setText("采样器状态");
    statueItem[1][0].setText("供样水状态");
    statueItem[2][0].setText("采样器报警状态");
    statueItem[3][0].setText("采样模式");
    statueItem[4][0].setText("门禁状态");
    statueItem[5][0].setText("门禁报警状态");
    statueItem[6][0].setText("门禁开关时间");
    statueItem[7][0].setText("门禁登录信息");
    //statueItem[7][0].setText("动态密码");

    statueItem[8][0].setText("超标留样时间");
    statueItem[9][0].setText("超标留样水样量");
    statueItem[10][0].setText("动态密码");

    for (int i=0;i<ui->tableStatus->rowCount();i++)
    {
        ui->tableStatus->setItem(i, 0, &statueItem[i][0]);
        ui->tableStatus->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        statueItem[i][1].setText("无数据");
        ui->tableStatus->setItem(i, 1, &statueItem[i][1]);
        ui->tableStatus->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    refreshComponent();
}


void FrameRealTimeData::refreshComponent()
{
     QString style="background: radial-gradient(circle, #10215C 0%, #070F29 100%);background-color: transparent;color:white;";
     QString styleUp = style + "image:url(:/Images/UP.png)";
     ui->lbUp->setStyleSheet(styleUp);
     QString styleDown = style + "image:url(:/Images/DOWN.png)";
     ui->lbDown->setStyleSheet(styleDown);

    if (currPage <= 1)
        ui->lbUp->setVisible(false);
    else
        ui->lbUp->setVisible(true);

    if (currPage>=totalPages)
        ui->lbDown->setVisible(false);
    else
        ui->lbDown->setVisible(true);

    //首先释放上一页显示的所有的组件
    for (int i=0;i<thermoList->count();i++)
        delete thermoList->at(i);
    thermoList->clear(); //清空列表

    int startC = (currPage-1)*(colsPerPage*rowsPerPage);
    int endC = currPage*(colsPerPage*rowsPerPage)<thermoTotals?currPage*(colsPerPage*rowsPerPage):thermoTotals;

    int i;
    for (i=startC;i<endC;i++)
    {
        FrameThermo * frameThermo = new FrameThermo( this->DataItemList->at(i));
        frameThermo->refreshValue();
        //frameThermo->setFixedSize(150, 200);
        ui->gridLayout->addWidget(frameThermo,(i%(colsPerPage*rowsPerPage))/colsPerPage, i%colsPerPage);
        thermoList->append(frameThermo);
    }

    //生成不使用的组件补空位
    if (i<currPage*(colsPerPage*rowsPerPage))
    {
        for (;i<currPage*(colsPerPage*rowsPerPage);i++)
        {
            FrameThermo * frameThermo = new FrameThermo();
            //frameThermo->setFixedSize(150, 200);
            frameThermo->setComponentVisible(false);
            ui->gridLayout->addWidget(frameThermo,(i%(colsPerPage*rowsPerPage))/colsPerPage, i%colsPerPage);
        }
    }

    //填充表格
    refreshTable();
    refreshTableStatus();
}


//翻页按钮的事件过滤器
bool FrameRealTimeData::eventFilter(QObject *obj, QEvent*event)
{
    if (event->type()==QEvent::MouseButtonPress)
    {
        if (obj==ui->lbUp)
        {
            currPage --;
        }
        else if (obj==ui->lbDown)
        {
            currPage ++;
        }
        refreshComponent();
        return true;
    }
    else
    {
        return QWidget::eventFilter(obj,event);
    }
}

//更新图表的槽函数
void FrameRealTimeData::updateFactorDataItem()
{

//qDebug()<<__FILE__<<":"<<__FUNCTION__<<endl;
    //更新图
    for (int i=0;i<thermoList->count();i++)
    {
        thermoList->at(i)->refreshValue();
    }
    //更新环境因子表
    refreshTable();
    //更新水质采样仪参数表
    refreshTableStatus();
}

//填充表格
void FrameRealTimeData::refreshTable()
{
    int st,et;
    st = (currPageTable-1)*rowsPerPageTable;
    et = currPageTable*rowsPerPageTable<totalItemsTable ? currPageTable*rowsPerPageTable : totalItemsTable;

    for (int i=st,j=0;i<et;i++)
    {
        if (this->EnvironmentItemList->at(i)->code == "ys01" ||
            this->EnvironmentItemList->at(i)->code == "ys02" )
        {
            int in,dn;
            bool rs = FactorDataType::getDataLength(this->EnvironmentItemList->at(i)->nType,in,dn);
            if (rs)
            {                
                if (dn==0)
                   envStatueItem[i][1].setText(QString::number(this->EnvironmentItemList->at(i)->value)+" "+this->EnvironmentItemList->at(i)->unitC);
                else
                   envStatueItem[i][1].setText(QString::number(this->EnvironmentItemList->at(i)->value,'f',dn)+" "+this->EnvironmentItemList->at(i)->unitC);
            }
            else
            {
                envStatueItem[i][1].setText(QString::number(this->EnvironmentItemList->at(i)->value)+" "+this->EnvironmentItemList->at(i)->unitC);
            }
            if (this->EnvironmentItemList->at(i)->value>=this->EnvironmentItemList->at(i)->warningValueMax ||
                this->EnvironmentItemList->at(i)->value<=this->EnvironmentItemList->at(i)->warningValueMin )
            {
                ui->tableDetail->item(i,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableDetail->item(i,1)->setTextColor(Qt::white);
            }
        }
        else if (this->EnvironmentItemList->at(i)->code == "ys03")
        {
            if (this->EnvironmentItemList->at(i)->value==0)
            {
                envStatueItem[i][1].setText("正常");
                ui->tableDetail->item(i,1)->setTextColor(Qt::white);
            }
            else
            {
                envStatueItem[i][1].setText("报警中");
                ui->tableDetail->item(i,1)->setTextColor(Qt::red);
            }
        }
        else if (this->EnvironmentItemList->at(i)->code == "ys04")
        {
            if (this->EnvironmentItemList->at(i)->value==0)
            {
                envStatueItem[i][1].setText("关门");
                ui->tableDetail->item(i,1)->setTextColor(Qt::white);
            }
            else
            {
                envStatueItem[i][1].setText("开门中");
                ui->tableDetail->item(i,1)->setTextColor(Qt::red);
            }
        }
        else if (this->EnvironmentItemList->at(i)->code == "ys05")
        {
            if (this->EnvironmentItemList->at(i)->value==0)
            {
                envStatueItem[i][1].setText("正常");
                ui->tableDetail->item(i,1)->setTextColor(Qt::white);
            }
            else
            {
                envStatueItem[i][1].setText("浸水中");
                ui->tableDetail->item(i,1)->setTextColor(Qt::red);
            }
        }
    }
}

void FrameRealTimeData::refreshTableStatus()
{
//    static int i=0;
//    this->wqs->wqs.sample_info.clear();
//    this->wqs->wqs.sample_info.insert("WaterSupplyFlag",i%2==0?"1":"0");
//    this->wqs->wqs.sample_info.insert("i42001",i%2==0?"运行":"故障");
//    this->wqs->wqs.sample_info.insert("i42002","无报警");
//    this->wqs->wqs.sample_info.insert("i42003","定时采样");
//    this->wqs->wqs.sample_info.insert("i42101","开");
//    this->wqs->wqs.sample_info.insert("i42102","正常");
//    this->wqs->wqs.sample_info.insert("i42103","运行");
//    this->wqs->wqs.sample_info.insert("i42105","20211204111111");
//    this->wqs->wqs.sample_info.insert("i42106","wqs");
//    this->wqs->wqs.sample_info.insert("i430xx_time","20211204111111");
//    this->wqs->wqs.sample_info.insert("i431xx","23.5");
//    i++;

//qDebug()<<__FILE__<<":"<<__FUNCTION__<<": WQS status:"<<endl;
//for(QMap<QString,QString>::ConstIterator ite=this->wqs->wqs.sample_info.constBegin(); ite!=this->wqs->wqs.sample_info.constEnd(); ++ite)
//    qDebug()<<"***"<<ite.key()<<": "<<ite.value()<<endl;

    if (this->wqs!=nullptr && this->wqs->wqs.sample_info.count()>0)
    {
        if (this->wqs->wqs.sample_info.value("i42001")!="")
           statueItem[0][1].setText(this->wqs->wqs.sample_info.value("i42001"));

        if (this->wqs->wqs.sample_info.value("WaterSupplyFlag")!="")
        {
            if (this->wqs->wqs.sample_info.value("WaterSupplyFlag")=="1")
                statueItem[1][1].setText("就绪");
            else
                statueItem[1][1].setText("未就绪");
        }

        if (this->wqs->wqs.sample_info.value("i42002")!="")
            statueItem[2][1].setText(this->wqs->wqs.sample_info.value("i42002"));

        if (this->wqs->wqs.sample_info.value("i42003")!="")
            statueItem[3][1].setText(this->wqs->wqs.sample_info.value("i42003"));

        if (this->wqs->wqs.sample_info.value("i42101")!="")
            statueItem[4][1].setText(this->wqs->wqs.sample_info.value("i42101"));

        if (this->wqs->wqs.sample_info.value("i42102")!="")
            statueItem[5][1].setText(this->wqs->wqs.sample_info.value("i42102"));

        if (this->wqs->wqs.sample_info.value("i42105")!="")
            statueItem[6][1].setText(this->wqs->wqs.sample_info.value("i42105"));

        if (this->wqs->wqs.sample_info.value("i42106")!="")
            statueItem[7][1].setText("868686");
        //statueItem[7][1].setText(this->wqs->wqs.sample_info.value("i42106"));

        //if (this->wqs->wqs.sample_info.value("i42103")!="")
        //    statueItem[7][1].setText(this->wqs->wqs.sample_info.value("i42103"));

        if (this->wqs->wqs.sample_info.value("i43002")!="")
            statueItem[8][1].setText(this->wqs->wqs.sample_info.value("i43002"));

        if (this->wqs->wqs.sample_info.value("i43003")!="")
            statueItem[9][1].setText(this->wqs->wqs.sample_info.value("i43003"));

        if (this->wqs->wqs.sample_info.value("i42103")!="")
            statueItem[10][1].setText(this->wqs->wqs.sample_info.value("i42103"));
    }
}

void FrameRealTimeData::on_tabWidget_currentChanged(int index)
{
    QString style1="QLabel{background-color:transparent;color:white;font:22px;width:60px}";
    QString style2="QLabel{background-color:transparent;color:gray;font:22px;width:60px}";
    if (index==0)
    {
        tabW->setStyleSheet(style1);
        tabE->setStyleSheet(style2);
    }
    else
    {
        tabW->setStyleSheet(style2);
        tabE->setStyleSheet(style1);
    }
}
/*****************************************新添加数据安全***************************/
void FrameRealTimeData::on_BtnDataSecurity_clicked()
{
    loginWindow->show();
}
