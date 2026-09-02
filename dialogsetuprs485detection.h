#ifndef DIALOGSETUPRS485DETECTION_H
#define DIALOGSETUPRS485DETECTION_H

#include <QDialog>
#include <QRegExpValidator>

#include <Headers/Model/MComArgs.h>

#include <Sources/DT/SamplerState.h>

#include <Headers/Tools/keyBoard.h>

namespace Ui {
class DialogSetupRS485Detection;
}

class DialogSetupRS485Detection : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupRS485Detection(QWidget *parent = nullptr);
    ~DialogSetupRS485Detection();

private slots:
    void on_btnClose_clicked();

    void on_btnCheck_clicked();

private:
    Ui::DialogSetupRS485Detection *ui;

    //SamplerState samplerstate;
    KeyBoard *keyBoard=nullptr;

    QRegExpValidator * pReg;

    void initComponents(int port);

    bool getSERIAL_PORT_CONFIG(QString portName,SERIAL_PORT_CONFIG &portConfig);

};

#endif // DIALOGSETUPRS485DETECTION_H
