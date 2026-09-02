#ifndef DIALOGSETUPWQS_H
#define DIALOGSETUPWQS_H

#include <QDialog>
#include <QRegExpValidator>
#include <Headers/Tools/keyBoard.h>

//定义水质采样器的内部编码
#define WQS "wqs"

namespace Ui {
class DialogSetupWQS;
}

class DialogSetupWQS : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupWQS(QWidget *parent = nullptr);
    ~DialogSetupWQS();

private slots:
    void on_btnClose_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupWQS *ui;
    KeyBoard *keyBoard=nullptr;
    QList<QString> portList;
    QRegExpValidator * pReg=nullptr;
signals:
    void toReboot();
};

#endif // DIALOGSETUPWQS_H
