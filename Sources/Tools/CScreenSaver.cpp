#include <Headers/Tools/CScreenSaver.h>

#include <QFile>
#include <QLabel>
#include <QEvent>
#include <QTimer>
#include <QPixmap>
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>

#include "Sources/Tools/net.h"

CScreenSaver::CScreenSaver(QObject *parent)
    : QObject(parent), waitInterval(WAIT_TIME)
{
    init();
}

CScreenSaver::~CScreenSaver()
{
}

void CScreenSaver::setScreenFrame(QWidget *frame)
{
    this->screenFrame = frame;
}

void CScreenSaver::setScreenTimeInterval(int time)
{
    this->waitInterval=time;
    timer->setInterval(this->waitInterval*1000);
    timer->start();
}

void CScreenSaver::init()
{

    //设置并启动timer。如果超过30s，则一直显示屏保并不再触发直到再次刷新定时器
    timer = new QTimer;
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    timer->start(waitInterval);

}

bool CScreenSaver::eventFilter(QObject *obj, QEvent *event)
{
    //判断事件类型
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::MouseMove
        || event->type() == QEvent::MouseButtonPress) {
        //有鼠标或键盘事件则重置timer
        timer->start();

        if (isLight==false)
        {
        //增加调亮屏幕背光
           BackLight_Light();
           isLight=true;
        }

    }

    return QObject::eventFilter(obj, event);
}

void CScreenSaver::slot_timeout()
{
    timer->stop();
    if (screenFrame!=nullptr)
        screenFrame->show();

    //增加调暗屏幕背光
     BackLight_Dark();

     isLight = false;
}
