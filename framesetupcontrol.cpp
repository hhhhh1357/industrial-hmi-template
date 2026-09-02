//#include "framesetupcontrol.h"
//#include "mainwindow.h"
//#include "ui_framesetupcontrol.h"

//#include <Headers/BLL/BFactor.h>

//#include <Sources/DT/plc.h>

//#include <QJsonArray>
//#include <QJsonDocument>

//extern MainWindow * mainWindow;
//extern PLC plc;

//FrameSetupControl::FrameSetupControl(QWidget *parent) :
//    QFrame(parent),
//    ui(new Ui::FrameSetupControl)
//{
//    ui->setupUi(this);

//    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

//    initComponent();

//    connect( this, SIGNAL(requestPLC(Task)), &plc, SLOT(requestHandler(Task)));

//}

//FrameSetupControl::~FrameSetupControl()
//{
//    delete ui;
//}

//void FrameSetupControl::initComponent()
//{
//    QString styleEnable="background-color: rgb(186, 189, 182);";
//    QString styleDisable="background-color: rgb(85, 87, 83);";

//    //读取数据库配置数据
//    BFactor bf;
//    QString args = bf.getArgsByCode("wqs");
//    QJsonDocument jsonDocument = QJsonDocument::fromJson(args.toLocal8Bit().data());
//    if (!(jsonDocument.isNull()))
//    {
//        QJsonObject argsJson = jsonDocument.object();
//        if (mainWindow->wqs==nullptr)
//        {
//            ui->BtnWaterSta->setEnabled(false);
//            ui->BtnWaterSto->setEnabled(false);
//            ui->BtnOverSam->setEnabled(false);
//            ui->BtnWaterSta->setStyleSheet(styleDisable);
//            ui->BtnWaterSto->setStyleSheet(styleDisable);
//            ui->BtnOverSam->setStyleSheet(styleDisable);
//        }
//        else
//        {
//            ui->BtnWaterSta->setEnabled(true);
//            ui->BtnWaterSto->setEnabled(true);
//            ui->BtnOverSam->setEnabled(true);
//            ui->BtnWaterSta->setStyleSheet(styleEnable);
//            ui->BtnWaterSto->setStyleSheet(styleEnable);
//            ui->BtnOverSam->setStyleSheet(styleEnable);
//        }

//        QString factorArgsJsonStr = argsJson.value("Factors").toString();
//        jsonDocument = QJsonDocument::fromJson(factorArgsJsonStr.toLocal8Bit().data());
//        if (!(jsonDocument.isNull()))
//        {
//            QJsonArray jsonArray = jsonDocument.array();
//            if ( !(jsonArray.isEmpty()) && jsonArray.count()>0)
//            {
//                for (int j=0;j<jsonArray.size();j++)
//                {
//                    if (jsonArray.at(j).toString()=="w01018")
//                    {
//                        ui->BtnCodSta->setEnabled(true);
//                        ui->BtnCodSto->setEnabled(true);
//                        ui->BtnCodSamCheck->setEnabled(true);
//                        ui->BtnInitCod->setEnabled(true);

//                        ui->BtnCodSta->setStyleSheet(styleEnable);
//                        ui->BtnCodSto->setStyleSheet(styleEnable);
//                        ui->BtnCodSamCheck->setStyleSheet(styleEnable);
//                        ui->BtnInitCod->setStyleSheet(styleEnable);
//                    }
//                    else if (jsonArray.at(j).toString()=="w21001")
//                    {
//                        ui->BtnDSta->setEnabled(true);
//                        ui->BtnDSto->setEnabled(true);
//                        ui->BtnDSamCheck->setEnabled(true);
//                        ui->BtnInitD->setEnabled(true);

//                        ui->BtnDSta->setStyleSheet(styleEnable);
//                        ui->BtnDSto->setStyleSheet(styleEnable);
//                        ui->BtnDSamCheck->setStyleSheet(styleEnable);
//                        ui->BtnInitD->setStyleSheet(styleEnable);
//                    }
////                    else if (jsonArray.at(j).toString()=="w21011")
////                    {
////                        ui->BtnLSta->setEnabled(true);
////                        ui->BtnLSto->setEnabled(true);
////                        ui->BtnLSamCheck->setEnabled(true);
////                        ui->BtnInitL->setEnabled(true);

////                        ui->BtnLSta->setStyleSheet(styleEnable);
////                        ui->BtnLSto->setStyleSheet(styleEnable);
////                        ui->BtnLSamCheck->setStyleSheet(styleEnable);
////                        ui->BtnInitL->setStyleSheet(styleEnable);
////                    }
////                    else if (jsonArray.at(j).toString()=="w21001")
////                    {
////                        ui->BtnDSta->setEnabled(true);
////                        ui->BtnDSto->setEnabled(true);
////                        ui->BtnDSamCheck->setEnabled(true);
////                        ui->BtnInitD->setEnabled(true);

////                        ui->BtnDSta->setStyleSheet(styleEnable);
////                        ui->BtnDSto->setStyleSheet(styleEnable);
////                        ui->BtnDSamCheck->setStyleSheet(styleEnable);
////                        ui->BtnInitD->setStyleSheet(styleEnable);
////                    }
//                    else if (jsonArray.at(j).toString()=="w21003")
//                    {
//                       ui->BtnADSta->setEnabled(true);
//                       ui->BtnADSto->setEnabled(true);
//                       ui->BtnADSamCheck->setEnabled(true);
//                       ui->BtnInitAD->setEnabled(true);

//                       ui->BtnADSta->setStyleSheet(styleEnable);
//                       ui->BtnADSto->setStyleSheet(styleEnable);
//                       ui->BtnADSamCheck->setStyleSheet(styleEnable);
//                       ui->BtnInitAD->setStyleSheet(styleEnable);
//                    }
//                    else if (jsonArray.at(j).toString()=="w21011")
//                    {
//                        ui->BtnLSta->setEnabled(true);
//                        ui->BtnLSto->setEnabled(true);
//                        ui->BtnLSamCheck->setEnabled(true);
//                        ui->BtnInitL->setEnabled(true);

//                        ui->BtnLSta->setStyleSheet(styleEnable);
//                        ui->BtnLSto->setStyleSheet(styleEnable);
//                        ui->BtnLSamCheck->setStyleSheet(styleEnable);
//                        ui->BtnInitL->setStyleSheet(styleEnable);
//                    }

//                }
//            }
//        }
//    }
//}

//bool FrameSetupControl::InitTask(QString factorCode, Task &task)
//{
//    MFactor factor;
//    BFactor bfactor;
//    bool r = bfactor.getFactorInfoByCode(factorCode,factor);

//    task.req.taskBegin = QDateTime::currentDateTime();
//    task.taskSource = TaskDirection::UI_REQUEST;
//    task.req.factorCode=factorCode;

//    if (factor.dataPortName.left(2)=="RS") //数字量端口
//    {
//        task.req.cmdCode = QString::number(factor.cmdCode+10000).right(2);
//        task.req.valueCount = factor.valueCount;
//        task.req.stationNo = (unsigned char) (factor.stationAddr);
//        task.req.plcAddr = factor.registerAddr;
//        if (factor.plcDataType=="H1234")
//           task.req.dataAlign = (DataAlign)1;
//        else if (factor.plcDataType=="H4321")
//           task.req.dataAlign = (DataAlign)0;
//        else if (factor.plcDataType=="H3412")
//           task.req.dataAlign = (DataAlign)2;
//        else
//           task.req.dataAlign = (DataAlign)2;

//        // 注意端口映射的计算
//        task.req.port =(URT_PORT)((factor.dataPortName.left(5)=="RS232")?factor.dataPortName.right(1).toInt()-1:factor.dataPortName.right(1).toInt()+4);
//    }
//    else if (factor.dataPortName.left(2)=="AI")  //模拟量
//    {
//        task.req.port = (URT_PORT)(factor.dataPortName.right(1).toInt() + 6); //模拟量通道端口编号：7-14
//    }

//    return r;
//}

//void FrameSetupControl::ControlToStart(QString factorCode)
//{

//    Task task;

//    if (InitTask( factorCode, task))
//    {
//        if (factorCode=="wqs")
//            task.req.action = PLCAction::CMD_SAMPLER_START;
//        else
//            task.req.action = PLCAction::CMD_ANALYZER_START;
//        emit requestPLC(task);
//    }
//}

//void FrameSetupControl::ControlToStop(QString factorCode)
//{
//    Task task;

//    if (InitTask( factorCode, task))
//    {
//        if (factorCode=="wqs")
//            task.req.action = PLCAction::CMD_SAMPLER_STOP;
//        else
//            task.req.action = PLCAction::CMD_ANALYZER_STOP;
//        emit requestPLC(task);
//    }
//}

//void FrameSetupControl::ControlToSampleCheck(QString factorCode)
//{
//    Task task;

//    if (InitTask( factorCode, task))
//    {
//        if (factorCode=="wqs")
//            task.req.action = PLCAction::CMD_SAMPLING_3015;
//        else
//            task.req.action = PLCAction::CMD_CHECK;
//        emit requestPLC(task);
//    }
//}

//void FrameSetupControl::ControlToInit(QString factorCode)
//{
//    Task task;

//    if (InitTask( factorCode, task))
//    {
//        task.req.action = PLCAction::CMD_INITA;
//        emit requestPLC(task);
//    }
//}

//void FrameSetupControl::on_BtnWaterSta_clicked()
//{
//    ControlToStart("wqs");
//}

//void FrameSetupControl::on_BtnCodSta_clicked()
//{
//    ControlToStart("w01018"); //COD
//}

//void FrameSetupControl::on_BtnLSta_clicked()
//{
//    ControlToStart("w21011"); //总磷
//}

//void FrameSetupControl::on_BtnDSta_clicked()
//{
//    ControlToStart("w21001");//总氮
//}

//void FrameSetupControl::on_BtnADSta_clicked()
//{
//    ControlToStart("w21003");//氨氮
//}

//void FrameSetupControl::on_BtnWaterSto_clicked()
//{
//    ControlToStop("wqs");
//}

//void FrameSetupControl::on_BtnCodSto_clicked()
//{
//    ControlToStop("w01018"); //COD
//}

//void FrameSetupControl::on_BtnLSto_clicked()
//{
//    ControlToStop("w21011"); //总磷
//}

//void FrameSetupControl::on_BtnDSto_clicked()
//{
//    ControlToStop("w21001");//总氮
//}
////氨氮
//void FrameSetupControl::on_BtnADSto_clicked()
//{
//    ControlToStop("w21003");//氨氮
//}

//void FrameSetupControl::on_BtnOverSam_clicked()
//{
//    ControlToSampleCheck("wqs");
//}

//void FrameSetupControl::on_BtnCodSamCheck_clicked()
//{
//    ControlToSampleCheck("w01018");//COD
//}

//void FrameSetupControl::on_BtnLSamCheck_clicked()
//{
//    ControlToSampleCheck("w21011");//总磷
//}

//void FrameSetupControl::on_BtnDSamCheck_clicked()
//{
//    ControlToSampleCheck("w21001");//总氮
//}

//void FrameSetupControl::on_BtnADSamCheck_clicked()
//{
//    ControlToSampleCheck("w21003");//氨氮
//}

//void FrameSetupControl::on_BtnInitCod_clicked()
//{
//    ControlToInit("w01018");//COD
//}

//void FrameSetupControl::on_BtnInitL_clicked()
//{
//    ControlToInit("w21011");//总磷
//}

//void FrameSetupControl::on_BtnInitD_clicked()
//{
//    ControlToInit("w21001");//总氮
//}

//void FrameSetupControl::on_BtnInitAD_clicked()
//{
//    ControlToInit("w21003");//氨氮
//}
