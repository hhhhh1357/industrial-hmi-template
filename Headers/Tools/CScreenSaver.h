#ifndef CSCREENSAVER
#define CSCREENSAVER

#ifndef QOBJECT_H
#include <QFrame>
#include <QObject>
#endif

class QTimer;
class QLabel;
class CScreenSaver : public QObject
{
    Q_OBJECT

public:
    CScreenSaver(QObject *parent = NULL);
    ~CScreenSaver();
    void setScreenFrame(QWidget * frame);

    void setScreenTimeInterval(int time);

protected slots:
    void slot_timeout();

protected:
    //初始化屏保参数
    void init();

    //事件接收处理函数，由installEventFilter调用方在接收到事件时调用
    bool eventFilter(QObject *watched, QEvent *event);

private:
    //定时器
    QTimer   *timer;

    //用于显示屏保图片的对象
    QLabel   *label;

    //初始屏保等待超时时间
    static const unsigned WAIT_TIME = 300000;

    unsigned waitInterval=WAIT_TIME;

    QWidget * screenFrame=nullptr;

    bool isLight=true;

};

#endif
