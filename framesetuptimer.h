#ifndef FRAMESETUPTIMER_H
#define FRAMESETUPTIMER_H

#include <QFrame>

namespace Ui {
class FrameSetupTimer;
}

class FrameSetupTimer : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSetupTimer(QWidget *parent = nullptr);
    ~FrameSetupTimer();

private slots:
//    void on_btnTimer_clicked();

//    void on_btnPwdSet_clicked();

    void on_btnRS232Check_clicked();

    void on_btnRS485Check_clicked();

    void on_btnAiChick_clicked();

//    void on_btnTimerSet_clicked();

private:
    Ui::FrameSetupTimer *ui;


};

#endif // FRAMESETUPTIMER_H
