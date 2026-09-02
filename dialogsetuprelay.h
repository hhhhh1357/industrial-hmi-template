#ifndef DIALOGSETUPRELAY_H
#define DIALOGSETUPRELAY_H

#include <QDialog>

#include "Headers/BLL/BComArgs.h"
#include "dialogsetupcommfactor.h"
#include "mainwindow.h"

namespace Ui {
class DialogSetupRelay;
}

class DialogSetupRelay : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupRelay(QWidget *parent = nullptr);
     DialogSetupRelay(QString comType,QWidget *parent = nullptr);
    ~DialogSetupRelay();
     BComArgs bca;

private slots:
     void on_btnClose_clicked();

     void on_btnSave_clicked();

     void on_rbCH1_clicked();

     void on_rbCH2_clicked();

     void on_rbCH3_clicked();

     void on_rbCH4_clicked();

     void on_checkBoxPortEnabled_stateChanged(int arg1);


private:
    Ui::DialogSetupRelay *ui;

    QString RelayType;

    MComArgs mca ;

    KeyBoard *keyBoard=nullptr;

    void initComponents(QString RelayType, int port);

    void setParamentComponentsEnable(bool enable);
signals:
    void toReboot();
};

#endif // DIALOGSETUPRELAY_H
