#include "dialogsetupfacsetting.h"
#include "mainwindow.h"
#include "ui_dialogsetupfacsetting.h"

#include <Headers/DAL/dal.h>

#include <QFile>

#include <Headers/BLL/BLocalService.h>

extern Sqlite bSqlite;
extern MainWindow * mainWindow;
extern BLocalService * localService;

DialogSetupFacSetting::DialogSetupFacSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupFacSetting)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #FFFFFF);");

    this->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时候，自动释放

    localService->pauseUploadRealData();
}

DialogSetupFacSetting::~DialogSetupFacSetting()
{
    delete ui;
    if (isRecovered==false)
    {
        localService->continueUploadRealData();
    }
}

void DialogSetupFacSetting::on_btnClose_clicked()
{
    close();
}

void DialogSetupFacSetting::on_btnSave_clicked()
{

    bSqlite.closeDB();
    QFile file;
    bool r = file.remove("industrialhmi.db");
    r = file.copy("industrialhmi_bak.db","industrialhmi.db");
    if (r)
    {
        isRecovered=true;
        mainWindow->reboot();
    }

}
