#ifndef FRAMESETUPOTHER_H
#define FRAMESETUPOTHER_H

#include <QFrame>

namespace Ui {
class FrameSetupOther;
}

class FrameSetupOther : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSetupOther(QWidget *parent = nullptr);
    ~FrameSetupOther();

private slots:
    void on_btnTimerSet_clicked();

    void on_btnPwdSet_clicked();

    void on_btnDogSet_clicked();

    void on_btnReFacSet_clicked();

    void on_btnScreenTIme_clicked();

    void on_btnPortRe_clicked();

    void on_btnInternetServerSet_clicked();

public slots:
    void setForceReboot();

signals:
    void toReboot();

private:
    Ui::FrameSetupOther *ui;

};

#endif // FRAMESETUPOTHER_H
