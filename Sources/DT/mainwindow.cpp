#include "SubWorkerBase.h"
#include "Task.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ConnectStatus.h"
#include "CloudTalker.h"

#include "CloudServices.h"

#include <QProcess>

extern CloudServices *cs;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //其次是连接

    /*
    qRegisterMetaType<Task>("Task");
    qRegisterMetaType<PLCRequest>("Request");
    qRegisterMetaType<PLCResponse>("Response");
    qRegisterMetaType<DataPacketData>("DataPacketData");
    qRegisterMetaType<SubWorkerBase>("SubWorkerBase");

    //qRegisterMetaType<QMap<QString, QAbstractSocket::SocketState>>("QMap");
    qRegisterMetaType<ConnectStatus>("ConnectStatus");*/

    connect(this, &MainWindow::requestCloudService, cs, &CloudServices::handRequest);
    connect(cs, &CloudServices::reportResult, this, &MainWindow::handleResult);
    connect(cs, &CloudServices::reportConnectionState, this, &MainWindow::handleConnectionState);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnCon_clicked()
{

    QString qnMsg = ui->msgBox->text().trimmed();

    Task task;
    task.taskSource = TaskDirection::SELF_REPORT;

    qDebug()<<__FILE__<<", "<<__func__<<", msg = "<<qnMsg<<endl;
    PackingTools pt;

    DataPacketData dpd = pt.DataPacketDataBuilder(qnMsg);
    bool isok = true;
    task.dataPacket.data = dpd;

    task.dataPacket.data.SelfReportCmd = qnMsg;
    emit requestCloudService(task);


    //模拟数据
//    for(int i=0; i<1; i++){
//        Task task;
//        task.taskSource = TaskDirection::SELF_REPORT;
//        PackingTools pt;
//        //2.1 产生对话阶段QN，
//        QDateTime dateTime = QDateTime::currentDateTime();

//        QString QN = dateTime.toString("yyyyMMddhhmmsszzz");
//        //2.2 产生DateTime
//        QString dt = dateTime.toString("yyyyMMddhhmmss");

//        // 产生两个模拟数据
//        QTime time;
//        time= QTime::currentTime();
//        qsrand(time.msec()+time.second()*1000);
//        int n1 = qrand() % 2;    //产生2以内的随机数
//        int n2 = qrand() % 2;

//        //QN=20160801085857223;ST=32;CN=2021;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&&DataTime=20160801085857;SB1-RS=1;SB2-RS=0&&
//        QString qnMsg = QString("QN=%1;ST=32;CN=2021;PW=%2;MN=%3;Flag=5;CP=&&DataTime=%4;SB1-RS=%5;SB2-RS=%6&&").arg(QN).arg("123456").arg("88880082086206").arg(dt).arg(n1).arg(n2);
//        DataPacketData dpd = pt.DataPacketDataBuilder(qnMsg);

//        task.dataPacket.data = dpd;
//        task.dataPacket.data.SelfReportCmd = qnMsg;
//        emit requestCloudService(task);
//        sleep(1);
//    }
}
//接收处理结果的槽
void MainWindow::handleResult(QString msg)
{
    ui->edtReturn->setText(msg);
}

void MainWindow::handleConnectionState(ConnectStatus connectionStates)
//void MainWindow::handleConnectionState()
{
    QString msg = "";
    msg += "云端服务器：" + connectionStates.ip + "，连接状态： " + QString::number(connectionStates.state);
    ui->edtConnectState->setText(msg);
}
