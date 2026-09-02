#include "dialogdiagram.h"
#include "framethermo.h"
#include "ui_framethermo.h"

#include <QPainter>

#include <Sources/Tools/datatype.h>

#include <Headers/BLL/BFactor.h>

FrameThermo::FrameThermo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameThermo)
{
    ui->setupUi(this);
    int inv = 55;
    w=tsize;   //圆柱宽
    h=height()-ui->lbTitle->height()-ui->lbUnit->height()-2*inv+15;  //圆柱高
    tleft = (width()-w)/2-10; //左
    ttop = ui->lbTitle->height()+inv+7;   //上
    color = QColor("#00FFFF");  //正常数值时的颜色

    ui->lbValueH->setGeometry(tleft-ui->lbValueH->width()-30,ttop-ui->lbValueH->height()/2,
                              ui->lbValueH->width(),ui->lbValueH->height());
    ui->lbValueL->setGeometry(tleft-ui->lbValueL->width()-30,ttop-ui->lbValueL->height()/2+h,
                              ui->lbValueL->width(),ui->lbValueL->height());
    ui->lbValueM->setGeometry(tleft-ui->lbValueM->width()-30,ttop-ui->lbValueM->height()/2+h/2,
                              ui->lbValueM->width(),ui->lbValueM->height());

    QString style="QFrame#FrameThermo{image: url(:/Images/smallfra.png);}";
    this->setStyleSheet(style);

}

FrameThermo::FrameThermo(MFactor *factor, QWidget *parent):FrameThermo(parent)
{
    this->factor = factor;
    int in,dn;
    FactorDataType::getDataLength(factor->nType,in,dn);

    this->valueMax = factor->maxValue;
    this->valueMin = factor->minValue;
    this->warningMax = factor->warningValueMax;
    this->warningMin = factor->warningValueMin;
    this->value = this->factor->value;
    this->title = this->factor->chName;
    this->unit = this->factor->unitC;

    ui->lbTitle->setText(title);
    ui->lbUnit->setText(unit);

    if (dn==0)
    {
        ui->lbValueL->setText(QString::number((int)valueMin));
        ui->lbValueH->setText(QString::number((int)valueMax));
        ui->lbValueM->setText(QString::number((int)((valueMax+valueMin)/2)));
    }
    else
    {
        ui->lbValueL->setText(QString::number(valueMin,'f',dn));
        ui->lbValueH->setText(QString::number(valueMax,'f',dn));
        ui->lbValueM->setText(QString::number((valueMax+valueMin)/2,'f',dn));
    }
    //安装事件过滤器

    this->installEventFilter(this);

    //refreshValue( );
}

FrameThermo::~FrameThermo()
{
    delete ui;
}

double FrameThermo::setValue(double value)
{
//    if (value>=123456789.0) //若为特殊值，就设为下限值
//        this->value = this->valueMin;
//    else
//        this->value = value;

    this->value = value;

    int ni=0,np=0;
    FactorDataType::getDataLength(this->factor->nType,ni,np);

    if (this->value==0)//123456789.0
        ui->lbValue->setText("");
    else
        ui->lbValue->setText(QString::number(this->factor->value,'f',np));
    this->repaint();
}

void FrameThermo::setComponentVisible(bool r)
{
    ui->lbTitle->setVisible(r);
    ui->lbUnit->setVisible(r);
    ui->lbValue->setVisible(r);
    ui->lbValueH->setVisible(r);
    ui->lbValueL->setVisible(r);
    ui->lbValueM->setVisible(r);
    isVisible = r;
}

void FrameThermo::refreshValue()
{
    isVisible = true;
    setValue(this->factor->value);
}

QString FrameThermo::getCode()
{
    return this->factor->code;
}

bool FrameThermo::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::MouseButtonPress)
    {
         DialogDiagram * dialog = new DialogDiagram(this->factor);
         dialog->show();
    }
}

void FrameThermo::paintEvent(QPaintEvent *)
{
    if (isVisible)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        drawCylinder(painter, color);
    }
}

void FrameThermo::drawCylinder(QPainter &painter, QColor color)
{
     //画线
     painter.save();
     QColor pcolor(Qt::gray);
     painter.setPen(QPen(pcolor,0.2));
     for (int i=0;i<5;i++)
        painter.drawLine(90,ttop+i*h/4,155,ttop+i*h/4);

     //画告警线
     QColor rcolor(Qt::red);
     painter.setPen(QPen(rcolor,0.6,Qt::DashLine));

     painter.drawLine(90,ttop+h-((warningMin-valueMin)*h/(valueMax-valueMin)),155,ttop+h-((warningMin-valueMin)*h/(valueMax-valueMin)));
     painter.drawLine(90,ttop+h-((warningMax-valueMin)*h/(valueMax-valueMin)),155,ttop+h-((warningMax-valueMin)*h/(valueMax-valueMin)));

     painter.restore();

     if (this->value==0)//123456789.0
         return;

     int step = (value-valueMin)*h/(valueMax-valueMin);

     //bottom 椭圆
     painter.save();
     QRectF bottomrect(tleft,ttop+h-tsize/4,tsize,tsize/2);
     QLinearGradient bottomlinear(tleft,ttop+h-tsize/4,tsize,tsize/2);

     if (value>=warningMax||value<=warningMin)
     {
         QColor wcolor("#980A0A");
         bottomlinear.setColorAt(0, wcolor);
         bottomlinear.setColorAt(0.3, wcolor.lighter(160));
         bottomlinear.setColorAt(0.35, wcolor.lighter(150));
         bottomlinear.setColorAt(0.98, wcolor.darker(200));
         bottomlinear.setColorAt(1, wcolor);
     }
     else
     {
         bottomlinear.setColorAt(0, color);
         bottomlinear.setColorAt(0.3, color.lighter(160));
         bottomlinear.setColorAt(0.35, color.lighter(150));
         bottomlinear.setColorAt(0.98, color.darker(200));
         bottomlinear.setColorAt(1, color);
     }
     bottomlinear.setStart(tleft,ttop+h);
     bottomlinear.setFinalStop(tleft+tsize,ttop+h);
     painter.setPen(Qt::NoPen);
     painter.setBrush(bottomlinear);
     painter.drawEllipse(bottomrect);
     painter.restore();

 /*
     //背景
     painter.save();
     QLinearGradient bgLinearGradient(tleft,ttop,tsize,h);
     bgLinearGradient.setColorAt(0, QColor("#A8C0D0"));
     bgLinearGradient.setColorAt(0.3, QColor("#CFDBE3"));
     bgLinearGradient.setColorAt(0.35, QColor("#A2BBCC").lighter(120));
     bgLinearGradient.setColorAt(0.98, QColor("#767775").lighter(120));
     bgLinearGradient.setColorAt(1, QColor("#A8C0D0"));
     bgLinearGradient.setStart(tleft,ttop);
     bgLinearGradient.setFinalStop(tleft+tsize,ttop+h);
     QRectF bgrect(tleft,ttop,tsize,h);
     painter.fillRect(bgrect, bgLinearGradient);
     painter.restore();
*/
     //中间
     painter.save();

     //设置值的位置和颜色
     ui->lbValue->setGeometry(tleft+tsize+25,ttop+h-step-ui->lbValue->height()/2,
                              ui->lbValue->width(),ui->lbValue->height());

     QRectF midrect(tleft, ttop+h-step, tsize,step);
     QLinearGradient midLinearGradient(tleft, ttop+h-step,tsize,step);

     QString valueStyle="background: radial-gradient(circle, #10215C 0%, #070F29 100%);background-color: transparent;";

     if (value>=warningMax||value<=warningMin)
     {
         valueStyle += "color:red;";
         ui->lbValue->setStyleSheet(valueStyle);
         QColor wcolor("#980A0A");
         midLinearGradient.setColorAt(0, wcolor);
         midLinearGradient.setColorAt(0.3, wcolor.lighter(160));
         midLinearGradient.setColorAt(0.35, wcolor.lighter(150));
         midLinearGradient.setColorAt(0.98, wcolor.darker(200));
         midLinearGradient.setColorAt(1, wcolor);
     }
     else
     {
         valueStyle+="color:white;";
         ui->lbValue->setStyleSheet(valueStyle);
         midLinearGradient.setColorAt(0, color);
         midLinearGradient.setColorAt(0.3, color.lighter(160));
         midLinearGradient.setColorAt(0.35, color.lighter(150));
         midLinearGradient.setColorAt(0.98, color.darker(200));
         midLinearGradient.setColorAt(1, color);
     }
     midLinearGradient.setStart(tleft,h);
     midLinearGradient.setFinalStop(tleft+tsize,h);
     painter.fillRect(midrect, midLinearGradient);
     painter.restore();

/*
     //top 椭圆
     painter.save();
     painter.setPen(Qt::NoPen);
     QLinearGradient fade3(tleft, ttop-tsize/4,tsize,tsize/2);
     fade3.setColorAt(0, QColor("#A8C0D0"));
    // fade3.setColorAt(0.3, QColor("#CFDBE3"));
     fade3.setColorAt(0.35, QColor("#A2BBCC").darker(100));
     fade3.setColorAt(0.9, QColor("#767775").darker(100));
     fade3.setColorAt(1, QColor("#A8C0D0"));
     fade3.setStart(tleft,ttop-tsize/4);
     fade3.setFinalStop(tleft+tsize,ttop+tsize/4);
     painter.setBrush(fade3);
     QRectF rectangle(tleft,ttop-tsize/4, tsize, tsize/2);
     painter.drawEllipse(rectangle);
     painter.restore();
     //////////////////////////////////////////////////////////////////////////
*/
    //mid 椭圆
     painter.save();
     QRectF midtoprect(tleft,ttop+h-step-tsize/4,tsize,tsize/2);
     QLinearGradient midtoplinear(tleft,ttop+h-step-tsize/4,tsize,step);

     if (value>=warningMax||value<=warningMin)
     {
         QColor wcolor("#980A0A");
         midtoplinear.setColorAt(0, wcolor.darker(100));
         midtoplinear.setColorAt(0.3, wcolor.darker(100));
         midtoplinear.setColorAt(0.35, wcolor.darker(100));
         midtoplinear.setColorAt(0.98, wcolor.darker(100));
         midtoplinear.setColorAt(1, wcolor.darker(100));
     }
     else
     {
         midtoplinear.setColorAt(0, color.darker(100));
         midtoplinear.setColorAt(0.3, color.darker(100));
         midtoplinear.setColorAt(0.35, color.darker(100));
         midtoplinear.setColorAt(0.98, color.darker(100));
         midtoplinear.setColorAt(1, color.darker(100));
     }
     midtoplinear.setStart(tleft,ttop+h-step-tsize/4);
     midtoplinear.setFinalStop(tleft+tsize,ttop+h-step+tsize/4);
     painter.setPen(Qt::NoPen);
     painter.setBrush(midtoplinear);
     painter.drawEllipse(midtoprect);
     painter.restore();
}


