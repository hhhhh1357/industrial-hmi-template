#ifndef DIALOGSETUPAI_H
#define DIALOGSETUPAI_H

#include <QDialog>
#include <QRegExpValidator>

//#include "Headers/BLL/BComArgs.h"
//#include "Headers/BLL/BFactor.h"
#include "dialogsetupcommfactor.h"
#include "mainwindow.h"

namespace Ui {
class DialogSetupAI;
}

class DialogSetupAI : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupAI(QWidget *parent = nullptr);
     DialogSetupAI(QString comType,QWidget *parent = nullptr);
    ~DialogSetupAI();

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

    void on_rbSignalTypeV_clicked();

    void on_rbSignalTypeA_clicked();

    void on_rbPort6_clicked();

    void on_rbPort7_clicked();

    void on_rbPort8_clicked();

    void on_factorC_textEdited(const QString &arg1);

private:
    Ui::DialogSetupAI *ui;

    QString AiType;

    MComArgs mca ;

    KeyBoard *keyBoard=nullptr;

    QRegExpValidator * pReg;

    QString portName;

    void initComponents(QString AiType, int port);

    void setComponentsEnable(bool enable);

    void setFactorParamentValue(QString factorCode);

    void getInuDnu(MFactor * factor, int & inu, int & dnu);

signals:
    void toReboot();

};

#endif // DIALOGSETUPAI_H
