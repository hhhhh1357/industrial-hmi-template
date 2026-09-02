#ifndef DIALOGSETUPNET_H
#define DIALOGSETUPNET_H

#include <QDialog>
#include <QRegExpValidator>
#include <Headers/BLL/BSysConfig.h>
#include "Headers/BLL/BComArgs.h"
#include "Headers/BLL/BFactor.h"
#include "dialogsetupcommfactor.h"
#include "mainwindow.h"

namespace Ui {
class DialogSetupNet;
}

class DialogSetupNet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupNet(QWidget *parent = nullptr);
    ~DialogSetupNet();

private slots:
    void on_btnSave_clicked();

    void on_btnClose_clicked();

    void on_checkBoxPortEnabled2_stateChanged(int arg1);

    void on_checkBoxPortEnabled3_stateChanged(int arg1);

    void on_chkBPortEnabled1_stateChanged(int arg1);

private:
    Ui::DialogSetupNet *ui;

    BSysConfig sysConfig;

    KeyBoard *keyBoard=nullptr;
    QRegExpValidator * pReg;

    void initComponents( );

    void setParamentComponentsEnable(QString PortName, bool enable);//设置相关组件无效

signals:
    void toReboot();
};

#endif // DIALOGSETUPNET_H
