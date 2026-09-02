#ifndef FRAMEREBOOTCOVER_H
#define FRAMEREBOOTCOVER_H

#include <QFrame>

#include <QThread>
#include <QTimer>

#include "formlogin.h"

namespace Ui {
class FrameRebootCover;
}

class FrameRebootCover : public QFrame
{
    Q_OBJECT

public:
    explicit FrameRebootCover(QWidget *parent = nullptr);
    FrameRebootCover(FormLogin * loginForm, QWidget *parent);
    ~FrameRebootCover();

    QTimer *sTimer;  //定时器
    FormLogin *loginForm;
    int t=0;

private:
    Ui::FrameRebootCover *ui;
};

#endif // FRAMEREBOOTCOVER_H
