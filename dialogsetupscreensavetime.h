#ifndef DIALOGSETUPSCREENSAVETIME_H
#define DIALOGSETUPSCREENSAVETIME_H

#include <QDialog>
#include <QRegExpValidator>

#include <Headers/Tools/keyBoard.h>

namespace Ui {
class DialogSetupScreenSaveTime;
}

class DialogSetupScreenSaveTime : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupScreenSaveTime(QWidget *parent = nullptr);
    ~DialogSetupScreenSaveTime();

private slots:
    void on_btnClose_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupScreenSaveTime *ui;

    KeyBoard *keyBoard=nullptr;

    QRegExpValidator * pReg;

signals:
    void toReboot();


};

#endif // DIALOGSETUPSCREENSAVETIME_H
