#ifndef DIALOGSETUPSITE_H
#define DIALOGSETUPSITE_H

#include <QDialog>

#include <QRegExpValidator>
#include "Headers/BLL/BComArgs.h"
#include "Headers/BLL/BFactor.h"
#include "dialogsetupcommfactor.h"
#include "mainwindow.h"

namespace Ui {
class DialogSetupSite;
}

class DialogSetupSite : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupSite(QWidget *parent = nullptr);
    ~DialogSetupSite();

private slots:
    void on_btnClose_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupSite *ui;

    //MComArgs * mca;

    KeyBoard *keyBoard=nullptr;
    QRegExpValidator * pReg;

    void initComponents( );
signals:
    void toReboot();

};

#endif // DIALOGSETUPSITE_H
