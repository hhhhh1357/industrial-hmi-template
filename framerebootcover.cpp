#include "framerebootcover.h"
#include "ui_framerebootcover.h"

FrameRebootCover::FrameRebootCover(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameRebootCover)
{
    ui->setupUi(this);

    this->sTimer = new QTimer();
    connect(this->sTimer,&QTimer::timeout,[=](){
             t++;
             if (t>=5)
             {
                this->sTimer->stop();
                this->setVisible(false);
                this->loginForm->show();
             }
        });

    this->sTimer->setInterval(1000);

    this->sTimer->start();
}

FrameRebootCover::FrameRebootCover(FormLogin *loginForm, QWidget *parent):FrameRebootCover(parent)
{
    this->loginForm = loginForm;
}

FrameRebootCover::~FrameRebootCover()
{
    delete ui;

    sTimer->stop();
    delete  sTimer;
}
