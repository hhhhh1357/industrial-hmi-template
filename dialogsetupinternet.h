#ifndef DIALOGSETUPINTERNET_H
#define DIALOGSETUPINTERNET_H

#include <QDialog>
#include <QRegExpValidator>
#include "Headers/Tools/keyBoard.h"


namespace Ui {
class DialogSetupInternet;
}

class DialogSetupInternet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupInternet(QWidget *parent = nullptr);
    ~DialogSetupInternet();

private slots:
    void on_btnClose_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupInternet *ui;

    KeyBoard *keyBoard=nullptr;
    QRegExpValidator * pReg1, * pReg2;

    void initComponents();

    bool validator(QString text);

signals:
    void toReboot();

};

#endif // DIALOGSETUPINTERNET_H
