#ifndef DIALOGSETUPRS232DETECTION_H
#define DIALOGSETUPRS232DETECTION_H

#include <QDialog>
#include <QRegExpValidator>

#include <Headers/Model/MComArgs.h>

#include <Sources/DT/SamplerState.h>

#include <Headers/Tools/keyBoard.h>

namespace Ui {
class DialogSetupRS232Detection;
}

class DialogSetupRS232Detection : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupRS232Detection(QWidget *parent = nullptr);
    ~DialogSetupRS232Detection();

private slots:

    void on_btnClose_clicked();

    void on_btnCheck_clicked();

private:
    Ui::DialogSetupRS232Detection *ui;
    KeyBoard *keyBoard=nullptr;

    QRegExpValidator * pReg;

    SamplerState samplerstate;
    bool getSERIAL_PORT_CONFIG(QString portName, SERIAL_PORT_CONFIG &portConfig);

};

#endif // DIALOGSETUPRS232DETECTION_H
