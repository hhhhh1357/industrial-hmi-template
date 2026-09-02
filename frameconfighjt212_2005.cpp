#include "frameconfighjt212_2005.h"
#include "ui_frameconfighjt212_2005.h"

FrameConfigHJT212_2005::FrameConfigHJT212_2005(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameConfigHJT212_2005)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);


}

FrameConfigHJT212_2005::~FrameConfigHJT212_2005()
{
    delete ui;
}

void FrameConfigHJT212_2005::on_btnClose_clicked()
{
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
    close();

}

void FrameConfigHJT212_2005::on_btnCancel_clicked()
{
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
    close();
}
