#ifndef DIALOGSETUPWATCHDOG_H
#define DIALOGSETUPWATCHDOG_H

#include <QDialog>
#include <QRegExpValidator>

#include <Headers/Tools/keyBoard.h>

namespace Ui {
class DialogSetupWatchDog;
}

class DialogSetupWatchDog : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupWatchDog(QWidget *parent = nullptr);
    ~DialogSetupWatchDog();

private slots:
    void on_btnClose_clicked();

    void on_btnSet_clicked();

private:
    Ui::DialogSetupWatchDog *ui;


        KeyBoard *keyBoard=nullptr;

        QRegExpValidator * pReg;

signals:
    void toReboot();

};

#endif // DIALOGSETUPWATCHDOG_H
