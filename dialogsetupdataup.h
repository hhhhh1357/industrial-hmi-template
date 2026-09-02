#ifndef DIALOGSETUPDATAUP_H
#define DIALOGSETUPDATAUP_H

#include <QDialog>
#include <QRegExpValidator>

#include <Headers/Tools/keyBoard.h>

#include <Headers/BLL/BSysDictionary.h>

namespace Ui {
class DialogSetupDataUp;
}

class DialogSetupDataUp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupDataUp(QWidget *parent = nullptr);
    ~DialogSetupDataUp();

private slots:


    void on_btnClose_clicked();

    void on_rbYes_Real_clicked();

    void on_rbNo_Real_clicked();

    void on_rbYes_Min_clicked();

    void on_rbNo_Min_clicked();

    void on_rbYes_Hour_clicked();

    void on_rbNo_Hour_clicked();

    void on_rbYes_Day_clicked();

    void on_rbNo_Day_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupDataUp *ui;

    KeyBoard *keyBoard=nullptr;
    QRegExpValidator * pReg;

    SysDictionary sysDic;

    void initComponents( );

signals:
    void toReboot();

};

#endif // DIALOGSETUPDATAUP_H
