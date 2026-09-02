#ifndef DIALOGSETUPAICHECK_H
#define DIALOGSETUPAICHECK_H

#include <QDialog>
#include <QRegExpValidator>

#include <Sources/DT/SamplerState.h>

#include <Headers/Tools/keyBoard.h>

namespace Ui {
class DialogSetupAiCheck;
}

class DialogSetupAiCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupAiCheck(QWidget *parent = nullptr);
    ~DialogSetupAiCheck();

private slots:


    void on_btnClose_clicked();

    void on_btnRead_clicked();

private:
    Ui::DialogSetupAiCheck *ui;

   // SamplerState samplerstate;

//    KeyBoard *keyBoard=nullptr;

//    QRegExpValidator * pReg;


    bool getSERIAL_PORT_CONFIG(QString portName,SERIAL_PORT_CONFIG &portConfig);


};

#endif // DIALOGSETUPAICHECK_H
