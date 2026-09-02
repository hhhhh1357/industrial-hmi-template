#include "dialogsetupdocheck.h"
#include "ui_dialogsetupdocheck.h"

DialogSetupDoCheck::DialogSetupDoCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupDoCheck)
{
    ui->setupUi(this);

//    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0093e9, stop:1 #80d0c7);");
   //  this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 gray, stop:1 #FFFFFF);");
     this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");
    //this->setLayout(ui->hLayoutMain);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
}

DialogSetupDoCheck::~DialogSetupDoCheck()
{
    delete ui;
}

void DialogSetupDoCheck::on_btnClose_clicked()
{
    close();
}
