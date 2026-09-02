#include "framerealexplaindata.h"
#include "ui_framerealexplaindata.h"

FrameRealExplainData::FrameRealExplainData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameRealExplainData)
{
    ui->setupUi(this);
}

FrameRealExplainData::~FrameRealExplainData()
{
    delete ui;
}
