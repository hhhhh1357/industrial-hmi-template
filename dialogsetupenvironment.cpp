#include "dialogsetupenvironment.h"
#include "ui_dialogsetupenvironment.h"

DialogSetupEnvironment::DialogSetupEnvironment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupEnvironment)
{
    ui->setupUi(this);
}

DialogSetupEnvironment::~DialogSetupEnvironment()
{
    delete ui;
}
