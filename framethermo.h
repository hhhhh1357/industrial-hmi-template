#ifndef FRAMETHERMO_H
#define FRAMETHERMO_H

#include <QFrame>

#include <Headers/Model/MFactor.h>

namespace Ui {
class FrameThermo;
}

class FrameThermo : public QFrame
{
    Q_OBJECT

public:
    explicit FrameThermo(QWidget *parent = nullptr);
    FrameThermo(MFactor * factor, QWidget *parent = nullptr);
    ~FrameThermo();

    double setValue(double value);
    void setComponentVisible(bool r);

    void refreshValue( );
    QString getCode();

private:
    Ui::FrameThermo *ui;

    bool isVisible=false;

    MFactor * factor;
    bool eventFilter(QObject *obj, QEvent*event);  //事件拦截器

    double warningMax, warningMin;
    double valueMax, valueMin;
    double value;
    QString title,unit;

    int w,h;
    int tleft, ttop;
    int tsize=24;
    QColor color;

    void paintEvent( QPaintEvent * );
    void drawCylinder( QPainter &painter, QColor color );
};

#endif // FRAMETHERMO_H
