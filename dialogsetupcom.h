#ifndef DIALOGSETUPCOM_H
#define DIALOGSETUPCOM_H

#include <QDialog>
#include "Headers/BLL/BComArgs.h"
#include "dialogsetupcommfactor.h"
#include "mainwindow.h"

namespace Ui {
class DialogSetupCom;
}

class DialogSetupCom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupCom(QWidget *parent = nullptr);
    DialogSetupCom(QString comType,QWidget *parent = nullptr);
    ~DialogSetupCom();

    BComArgs bca;

private slots:


    void on_btnClose_clicked();

    void on_btnSave_clicked();

    void on_rbPort5_clicked();

    void on_rbPort4_clicked();

    void on_rbPort3_clicked();

    void on_rbPort2_clicked();

    void on_rbPort1_clicked();

    void on_checkBoxPortEnabled_stateChanged(int arg1);

    void on_btnBrowse_clicked();

    void updateFactors(QString factors);

private:
    Ui::DialogSetupCom *ui;

    QString ComType;

    MComArgs * mcaPtr ;
    MComArgs mmca;

    void initComponents(QString comType, int port);

    void setParamentComponentsEnable(bool enable);

signals:
    void toReboot();


};

#endif // DIALOGSETUPCOM_H
