
#ifndef DIALOGSETUPSWITCH_H
#define DIALOGSETUPSWITCH_H

#include <QDialog>

#include "Headers/BLL/BComArgs.h"
#include "dialogsetupcommfactor.h"
#include "mainwindow.h"

namespace Ui {
class DialogSetupSwitch;
}

class DialogSetupSwitch : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupSwitch(QWidget *parent = nullptr);
     DialogSetupSwitch(QString comType,QWidget *parent = nullptr);
    ~DialogSetupSwitch();

    BComArgs bca;

private slots:
     void on_rbPort1_clicked();

     void on_btnClose_clicked();

     void on_rbPort2_clicked();

     void on_rbPort3_clicked();

     void on_rbPort4_clicked();



     void on_btnSave_clicked();

     void on_checkBoxPortEnabled_stateChanged(int arg1);



private:
    Ui::DialogSetupSwitch *ui;

    QString SwitchType;

    MComArgs * mca ;

    KeyBoard *keyBoard=nullptr;

    void initComponents(QString SwitchType, int port);

    void setParamentComponentsEnable(bool enable);
signals:
    void toReboot();
};

#endif // DIALOGSETUPSWITCH_H

