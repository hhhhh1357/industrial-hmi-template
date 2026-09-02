#include "formlogin.h"
#include "ui_formlogin.h"

#include "Headers/BLL/BUser.h"

FormLogin::FormLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLogin)
{
    ui->setupUi(this);
     setComponent();
}

FormLogin::~FormLogin()
{
    delete ui;
}

void FormLogin::setComponent()
{
   //设置相关组件透明/颜色
    ui->lineEditPwd->setEnabled(true);
    ui->lineEditPwd->setFocus();
}

void FormLogin::on_btnEnter_clicked()
{
    BUser bu ;
    if (bu.login("admin",ui->lineEditPwd->text()))
    {
       if (mainWindow->isHidden())
           mainWindow->show();
       //this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
       //close();
       ui->lineEditPwd->setText("");
       this->hide();
    }
    else
    {
        ui->lineEditPwd->setText("");
    }
}

void FormLogin::on_btn_1_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"1");
}

void FormLogin::on_btn_2_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"2");
}

void FormLogin::on_btn_3_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"3");
}

void FormLogin::on_btn_4_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"4");
}

void FormLogin::on_btn_5_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"5");
}

void FormLogin::on_btn_6_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"6");
}

void FormLogin::on_btn_7_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"7");
}

void FormLogin::on_btn_8_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"8");
}

void FormLogin::on_btn_9_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"9");
}

void FormLogin::on_btn_0_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"0");
}

void FormLogin::on_btnEsc_clicked()
{
    ui->lineEditPwd->setText("");
}

void FormLogin::on_btnDel_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text().left(ui->lineEditPwd->text().length()-1));
}

void FormLogin::on_btnStar_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"*");
}

void FormLogin::on_btnSharp_clicked()
{
    ui->lineEditPwd->setText(ui->lineEditPwd->text()+"#");
}
