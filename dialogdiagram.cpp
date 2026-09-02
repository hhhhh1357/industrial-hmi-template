#include "dialogdiagram.h"
#include "ui_dialogdiagram.h"

DialogDiagram::DialogDiagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDiagram)
{
    ui->setupUi(this);
    //this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0093e9, stop:1 #80d0c7);");

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放
}

DialogDiagram::DialogDiagram(MFactor *factor, QWidget *parent):DialogDiagram(parent)
{
    this->factor = factor;
    //ui->lbTitle->setText(factor->chName+" ( "+factor->unitC+" )");
    frameDiagram = new FrameDiagram(factor, ui->frame);
    frameDiagram->show();
}

DialogDiagram::~DialogDiagram()
{
   // delete frameDiagram;
    delete ui;
}

void DialogDiagram::on_btnClose_clicked()
{
    close();
}
