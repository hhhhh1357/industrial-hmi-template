#include "dialogsetupnet.h"
#include "ui_dialogsetupnet.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "Sources/Tools/net.h"

#include <Headers/BLL/BHistoryData.h>

#include <Sources/Tools/sysutil.h>

extern MainWindow * mainWindow;

DialogSetupNet::DialogSetupNet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupNet)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放


    //设置只能输入数字和小数点
    QRegExp rx("[0-9.0-9]*");
    pReg = new QRegExpValidator(rx,this);
    ui->dataIpAddr1->setValidator(pReg);
    ui->dataIpAddr2->setValidator(pReg);

    ui->dataSubMask1->setValidator(pReg);
    ui->dataSubMask2->setValidator(pReg);

    ui->dataNetWay1->setValidator(pReg);
    ui->dataNetWay2->setValidator(pReg);

    initComponents();

    keyBoard = new KeyBoard(this);
    keyBoard->changeStyle(2);
    keyBoard->changeType("符");

    ui->btnClose->setFocus();

}

DialogSetupNet::~DialogSetupNet()
{
    delete ui;
    delete pReg;
    if (keyBoard!=nullptr)
       delete keyBoard;
}

void DialogSetupNet::initComponents()
{

    if (sysConfig.EthernetList.at(0)->status!=UNUSED)
    {
        ui->chkBPortEnabled1->setChecked(true);

        ui->dataIpAddr1->setEnabled(true);
        ui->dataSubMask1->setEnabled(true);
        ui->dataNetWay1->setEnabled(true);

        ui->dataIpAddr1->setText(sysConfig.EthernetList.at(0)->ip);
        ui->dataSubMask1->setText(sysConfig.EthernetList.at(0)->mask);
        ui->dataNetWay1->setText(sysConfig.EthernetList.at(0)->gate);
    }
    else
    {
        ui->dataIpAddr1->setEnabled(false);
        ui->dataSubMask1->setEnabled(false);
        ui->dataNetWay1->setEnabled(false);
        ui->chkBPortEnabled1->setChecked(false);
    }

    if (sysConfig.EthernetList.at(1)->status!=UNUSED)
    {
        ui->checkBoxPortEnabled2->setChecked(true);

        ui->dataIpAddr2->setEnabled(true);
        ui->dataSubMask2->setEnabled(true);
        ui->dataNetWay2->setEnabled(true);

        ui->dataIpAddr2->setText(sysConfig.EthernetList.at(1)->ip);
        ui->dataSubMask2->setText(sysConfig.EthernetList.at(1)->mask);
        ui->dataNetWay2->setText(sysConfig.EthernetList.at(1)->gate);
    }
    else
    {
        ui->checkBoxPortEnabled2->setChecked(false);
        ui->dataIpAddr2->setEnabled(false);
        ui->dataSubMask2->setEnabled(false);
        ui->dataNetWay2->setEnabled(false);
    }
//******************20220606添加4G-DTU初始化************************//
//    if(sysConfig.EthernetList.at(2)->status!=UNUSED){
//        ui->checkBoxPortEnabled3->setChecked(true);

//        if (net.provider=="ChinaTelecom")
//            ui->rbChinaTelecom->setChecked(true);
//        else if (sysConfig.dtu4G.provider=="ChinaMobile")
//            ui->rbChinaMobile->setChecked(true);
//        else
//            ui->rbChinaUnicom->setChecked(true);
//    }
//    else{
//        ui->checkBoxPortEnabled3->setChecked(false);
//        ui->rbChinaMobile->setEnabled(false);
//        ui->rbChinaUnicom->setEnabled(false);
//        ui->rbChinaTelecom->setEnabled(false);
//    }

    //****************2022-5-22添加***********************//



//****************************冯老师代码***************************//
    if (sysConfig.dtu4G.enabled)
    {
        ui->checkBoxPortEnabled3->setChecked(true);
        if (sysConfig.dtu4G.provider=="ChinaTelecom")
            ui->rbChinaTelecom->setChecked(true);
        else if (sysConfig.dtu4G.provider=="ChinaMobile")
            ui->rbChinaMobile->setChecked(true);
        else
            ui->rbChinaUnicom->setChecked(true);

    }
    else
    {
        ui->checkBoxPortEnabled3->setChecked(false);
        ui->rbChinaMobile->setEnabled(false);
        ui->rbChinaUnicom->setEnabled(false);
        ui->rbChinaTelecom->setEnabled(false);
    }

}

void DialogSetupNet::setParamentComponentsEnable(QString PortName,bool enable)
{
     QString style, netPortcolor;
    if(PortName=="NETPORT-1")
    {
        /******************************网口1是否启用组件显示状态**********************************/
        if (enable==false)
        {
            ui->dataIpAddr1->setText("");
            ui->dataSubMask1->setText("");
            ui->dataNetWay1->setText("");
            ui->dataIpAddr1->setEnabled(false);
            ui->dataSubMask1->setEnabled(false);
            ui->dataNetWay1->setEnabled(false);
        }
        else
        {
            ui->dataIpAddr1->setEnabled(true);
            ui->dataSubMask1->setEnabled(true);
            ui->dataNetWay1->setEnabled(true);
            ui->dataIpAddr1->setText(sysConfig.EthernetList.at(0)->ip);
            ui->dataSubMask1->setText(sysConfig.EthernetList.at(0)->mask);
            ui->dataNetWay1->setText(sysConfig.EthernetList.at(0)->gate);
        }
    }
    else if(PortName=="NETPORT-2")
    {
         /******************************网口2是否启用组件显示状态**********************************/
        if (enable==false)
        {
            ui->dataIpAddr2->setText("");
            ui->dataSubMask2->setText("");
            ui->dataNetWay2->setText("");
            ui->dataIpAddr2->setEnabled(false);
            ui->dataSubMask2->setEnabled(false);
            ui->dataNetWay2->setEnabled(false);
        }
        else
        {
            ui->dataIpAddr2->setEnabled(true);
            ui->dataSubMask2->setEnabled(true);
            ui->dataNetWay2->setEnabled(true);
            ui->dataIpAddr2->setText(sysConfig.EthernetList.at(1)->ip);
            ui->dataSubMask2->setText(sysConfig.EthernetList.at(1)->mask);
            ui->dataNetWay2->setText(sysConfig.EthernetList.at(1)->gate);
        }
    }

//********************20220606添加4G是否启用组件显示状态************//
//    else if(PortName=="4G-DTU-1")
//       {
//           if(enable==false){
//               ui->rbChinaMobile->setChecked(false);
//               ui->rbChinaUnicom->setChecked(false);
//               ui->rbChinaTelecom->setChecked(false);
//           }
//           else{
//               if (sysConfig.dtu4G.provider=="ChinaMobile")
//                   ui->rbChinaMobile->setChecked(true);
//               else if(sysConfig.dtu4G.provider=="ChinaUnicom")
//                   ui->rbChinaUnicom->setChecked(true);
//               else
//                   ui->rbChinaTelecom->setChecked(true);
//           }
//       }

}

void DialogSetupNet::on_btnSave_clicked()
{
    bool r = true;
//*********************冯老师的代码部分**************************//
    sysConfig.dtu4G.enabled = ui->checkBoxPortEnabled3->isChecked();
    if (sysConfig.dtu4G.enabled)
        sysConfig.dtu4G.status = 1;
    else
        sysConfig.dtu4G.status = 0;
    if (ui->rbChinaTelecom->isChecked())
        sysConfig.dtu4G.provider = "ChinaTelecom";
    else if (ui->rbChinaMobile->isChecked())
        sysConfig.dtu4G.provider = "ChinaMobile";
    else
        sysConfig.dtu4G.provider = "ChinaUnicom";
    r = r && sysConfig.save4GDtu(sysConfig.dtu4G);
    if (sysConfig.dtu4G.enabled)
        r = r &&SysUtil::dial4GDtu(sysConfig.dtu4G.provider);

    Ethernet net;
    net.portName = "NETPORT-1";
    net.ip=ui->dataIpAddr1->text();
    net.mask=ui->dataSubMask1->text();
    net.gate=ui->dataNetWay1->text();
    net.status = ui->chkBPortEnabled1->isChecked()?1:0;
    r = r && sysConfig.saveEthernetArgs(net);
    if(ui->chkBPortEnabled1->isChecked())  //网口启用
    {
        r = r &&SysUtil::setEthernet(net.portName,net.ip,net.mask,net.gate);
    }
    else  //网口不启用
    {
        SysUtil::closeNetwork(net.portName);
    }

    net.portName = "NETPORT-2";
    net.ip=ui->dataIpAddr2->text();
    net.mask=ui->dataSubMask2->text();
    net.gate=ui->dataNetWay2->text();
    net.status = ui->checkBoxPortEnabled2->isChecked()?1:0;
    r = r && sysConfig.saveEthernetArgs(net);
    if(ui->checkBoxPortEnabled2->isChecked())
    {
        r = r && SysUtil::setEthernet(net.portName,net.ip,net.mask,net.gate);
    }
    else  //网口不启用
    {
        SysUtil::closeNetwork(net.portName);
    }
//***************************20220606测试4G保存时新添加*****************************//
//    net.portName="4G-DTU-1";
//    net.enabled=ui->checkBoxPortEnabled3->isChecked();
//    if(net.enabled)
//        net.Status=1;
//    else
//        net.Status=0;
//    if (ui->rbChinaTelecom->isChecked())
//        net.provider = "ChinaTelecom";
//    else if (ui->rbChinaMobile->isChecked())
//        net.provider = "ChinaMobile";
//    else
//        net.provider = "ChinaUnicom";
//    r = r && sysConfig.save4GDtu(net);
//    if (net.enabled)
//        r = r &&SysUtil::dial4GDtu(net.provider);

//********************************************************************************//
    if ( r )
    {
        QMessageBox messageBox(QMessageBox::NoIcon, "信息提示", "网络参数保存成功！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox(QMessageBox::Warning, "警告", "网络参数保存失败！", QMessageBox::Ok, nullptr);
        messageBox.exec();
    }
    emit toReboot();
}

void DialogSetupNet::on_btnClose_clicked()
{
    close();
}

void DialogSetupNet::on_checkBoxPortEnabled2_stateChanged(int arg1)
{
    /*******************************************************网口2设置相关组件是否启用*****************************************************/
    if (arg1==2)
    {
        if(QMessageBox::Yes==QMessageBox::question(nullptr,tr("提示!"),tr("确定此网口启用吗？"),
                                                  QMessageBox::No|QMessageBox::Yes))
        {
             setParamentComponentsEnable("NETPORT-2",true);
             ui->checkBoxPortEnabled2->setText("已启用");

             ui->checkBoxPortEnabled3->setChecked(false);
             ui->rbChinaMobile->setEnabled(false);
             ui->rbChinaUnicom->setEnabled(false);
             ui->rbChinaTelecom->setEnabled(false);
        }
        else
        {
            setParamentComponentsEnable("NETPORT-2", false);//设置相关组件无效
            ui->checkBoxPortEnabled2->setText("未启用");
        }
    }
    else
    {
        // 相关组件设置成 无效
        setParamentComponentsEnable("NETPORT-2",false);
        ui->checkBoxPortEnabled2->setText("未启用");
    }
}

void DialogSetupNet::on_checkBoxPortEnabled3_stateChanged(int arg1)
{
    /*******************************************************4G-DTU设置相关组件是否启用********************************************************/
    if (arg1==2)
    {
        if(QMessageBox::Yes==QMessageBox::question(nullptr,tr("提示!"),tr("确定启用 4G DTU 吗？"),
                                                  QMessageBox::No|QMessageBox::Yes))
        {
             //setParamentComponentsEnable("4G-DTU",true);//*******************2022-5-22添加*******//
             ui->checkBoxPortEnabled3->setText("已启用");

             ui->chkBPortEnabled1->setChecked(false);
             ui->checkBoxPortEnabled2->setChecked(false);
             ui->rbChinaMobile->setEnabled(true);
             ui->rbChinaUnicom->setEnabled(true);
             ui->rbChinaTelecom->setEnabled(true);

        }
        else
        {
            //setParamentComponentsEnable("4G-DTU", false);//设置相关组件无效//*******************2022-5-22添加*******//
            ui->checkBoxPortEnabled3->setText("未启用");
            //************************************加2020-5-22*******************************************//
            ui->rbChinaMobile->setEnabled(false);
            ui->rbChinaUnicom->setEnabled(false);
            ui->rbChinaTelecom->setEnabled(false);
        }
    }
    else
    {
        // 相关组件设置成 无效
        //setParamentComponentsEnable("4G-DTU",false);//*******************2022-5-22添加*******//
        ui->checkBoxPortEnabled3->setText("未启用");
        //**********************2022-5-21***************************************//
        ui->rbChinaMobile->setEnabled(false);
        ui->rbChinaUnicom->setEnabled(false);
        ui->rbChinaTelecom->setEnabled(false);

    }
}

void DialogSetupNet::on_chkBPortEnabled1_stateChanged(int arg1)
{
    /*****************************************网口1设置相关组件是否启用*****************************************************************/
    if (arg1==2)
    {
        if(QMessageBox::Yes==QMessageBox::question(nullptr,tr("提示!"),tr("确定此网口启用吗？"),
                                                  QMessageBox::No|QMessageBox::Yes))
        {
             setParamentComponentsEnable("NETPORT-1",true);
             ui->chkBPortEnabled1->setText("已启用");
\
             ui->checkBoxPortEnabled3->setChecked(false);
             ui->rbChinaMobile->setEnabled(false);
             ui->rbChinaUnicom->setEnabled(false);
             ui->rbChinaTelecom->setEnabled(false);
        }
        else
        {
            setParamentComponentsEnable("NETPORT-1", false);//设置相关组件无效
            ui->chkBPortEnabled1->setText("未启用");
        }
    }
    else
    {
        // 相关组件设置成 无效
        setParamentComponentsEnable("NETPORT-1",false);
        ui->chkBPortEnabled1->setText("未启用");
    }
}
