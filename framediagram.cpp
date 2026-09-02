#include "framediagram.h"
#include "ui_framediagram.h"

#include <Headers/BLL/BSysDictionary.h>
#include <qwt_math.h>

FrameDiagram::FrameDiagram(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameDiagram)
{
    ui->setupUi(this);

    this->setLayout(ui->vLayout);

    //X 轴显示为时间
    timeScale = new TimeScakeDrawR();
    ui->qwtPlot->setAxisScaleDraw(QwtPlot::xBottom, timeScale);
    //设置网格线
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );
    grid->enableY( true );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach( ui->qwtPlot );
}

FrameDiagram::FrameDiagram(MFactor *factor, QWidget *parent):FrameDiagram(parent)
{
    this->factor = factor;
    curve = new QwtPlotCurve();
    curve->setPen(Qt::blue,2);   //设置曲线颜色 粗细
    curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化    

    curveMax = new QwtPlotCurve();
    curveMin = new QwtPlotCurve();
    curveMax->setPen(Qt::red,2,Qt::DotLine);
    curveMin->setPen(Qt::red,2,Qt::DotLine);

    yScale = (factor->maxValue-factor->minValue)/10;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,factor->minValue,factor->maxValue,yScale);
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,factor->chName+" ("+factor->unitC+")");

    SysDictionary sysDic;
    int interval = sysDic.getINTERVAL();
    quint64 max = factor->valueTimestamp.toMSecsSinceEpoch();
    quint64 min = max - showDataLen*interval*1000;
    xScale = interval*1000*30;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,min,max,xScale);

    for (int i=showDataLen;i>0;i--)
    {
        values.append((factor->maxValue+factor->minValue)/2);
        timestamps.append(max-i*interval*1000);
        warningValueMax.append(factor->warningValueMax);
        warningValueMin.append(factor->warningValueMin);
    }

    setUpdateTimer();
}

//设置显示时间的定时器
void FrameDiagram::setUpdateTimer()
{
    updateTimer = new QTimer();
    connect(updateTimer,&QTimer::timeout,[=](){
          quint64 n1,n2;
          n1=factor->valueTimestamp.toMSecsSinceEpoch();
          n2=timestamps.at(timestamps.size()-1);
          if (n1>n2)
          {
              timestamps.erase(timestamps.begin(),timestamps.begin()+1);
              timestamps.append(factor->valueTimestamp.toMSecsSinceEpoch());

              values.erase(values.begin(),values.begin()+1);
              values.append(factor->value);

              ui->qwtPlot->setAxisScale(QwtPlot::xBottom,timestamps.at(0),timestamps.at(timestamps.size()-1),xScale);
              curve->setSamples(timestamps, values);
              curve->attach(ui->qwtPlot);

              curveMax->setSamples(timestamps,warningValueMax);
              curveMin->setSamples(timestamps,warningValueMin);
              curveMax->attach(ui->qwtPlot);
              curveMin->attach(ui->qwtPlot);

              ui->qwtPlot->replot();
          }
        });

    updateTimer->setInterval(1000);  //设置定时器启动的时间间隔为 1 秒
    if (updateTimer->isActive() == false)
    {
           updateTimer->start();
    }
}

FrameDiagram::~FrameDiagram()
{
    if (updateTimer!=nullptr) delete updateTimer;
    delete ui;
}

void FrameDiagram::setDiagramArgs(QString factorName, QString unit, QVector<double> values, QVector<double> timestamps,
                                  double valueMax, double valueMin, double wValueMax, double wValueMin)
{
    //清除画的线
    if (curveVMax!=nullptr)
    {
        curveVMax->detach();
        curveVMin->detach();
        curveVAvg->detach();
    }

    if (curve==nullptr)
    {
        curve = new QwtPlotCurve();
        curve->setPen(Qt::yellow,2);   //设置曲线颜色 粗细
        curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化

        QString style="color:white;";
        ui->qwtPlot->setStyleSheet(style);

        curveMax = new QwtPlotCurve();
        curveMin = new QwtPlotCurve();
        curveMax->setPen(Qt::red,2,Qt::DotLine);
        curveMin->setPen(Qt::red,2,Qt::DotLine);
    }
    warningValueMax.clear();
    warningValueMin.clear();

    showDataLen = timestamps.size();

    for (int i=showDataLen;i>0;i--)
    {
        this->warningValueMax.append(wValueMax);
        this->warningValueMin.append(wValueMin);
    }

    if (valueMax==0 && valueMin==0)
    {
        valueMax = wValueMax + (wValueMax-wValueMin)/4;
        valueMin = wValueMin - (wValueMax-wValueMin)/4;
    }

    yScale = (valueMax-valueMin)/10;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,valueMin,valueMax,yScale);
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,factorName+" ("+unit+")");

    dataEnd = timestamps.size()-1;
    if (dataEnd+1>showDataLen)
        dataStart = dataEnd-showDataLen+1;
    else
        dataStart = 0;

    long long dt = (timestamps.at(timestamps.size()-1)-timestamps.at(0));
    if (dt/showDataLen > 59*60*1000)
    {
        dataType = 3;
        xScale = 60*60*1000*(dt/(60*60*1000))/showDataLen ;
    }
    else if (dt/showDataLen > 59*1000)
    {
        dataType = 2;
        xScale = 60*1000*(dt/(60*1000))*10/showDataLen ;
    }
    else
    {
        dataType = 1;
        xScale = 60*1000*(dt/(60*1000))*5/showDataLen ;
    }

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,timestamps.at(dataStart),timestamps.at(dataEnd),xScale);

    curve->setSamples(timestamps, values);
    curve->attach(ui->qwtPlot);

    curveMax->setSamples(timestamps,this->warningValueMax);
    curveMin->setSamples(timestamps,this->warningValueMin);
    curveMax->attach(ui->qwtPlot);
    curveMin->attach(ui->qwtPlot);
    ui->qwtPlot->replot();
}

void FrameDiagram::setDiagramArgs(QString factorName, QString unit, QVector<double> valuesMax,
                                  QVector<double> valuesMin, QVector<double> valuesAvg,
                                  QVector<double> timestamps)
{
    //清除画的线
    if (curve!=nullptr)
    {
        warningValueMax.clear();
        warningValueMin.clear();
        values.clear();
        curveMax->detach();
        curveMin->detach();
        curve->detach();
    }

    if (curveVMax==nullptr)
    {
        QString style="color:white;";
        ui->qwtPlot->setStyleSheet(style);

        curveVMax = new QwtPlotCurve();
        curveVMax->setPen(Qt::green,2);   //设置曲线颜色 粗细
        curveVMax->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
        curveVMin = new QwtPlotCurve();
        curveVMin->setPen(Qt::yellow,2);   //设置曲线颜色 粗细
        curveVMin->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
        curveVAvg = new QwtPlotCurve();
        curveVAvg->setPen(Qt::blue,2);   //设置曲线颜色 粗细
        curveVAvg->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    }
    showDataLen = timestamps.size();

    double vMax, vMin;
    vMax=valuesMax.at(0);
    for (auto v:valuesMax)
        if(v>vMax) vMax = v;
    vMin=valuesMin.at(0);
    for (auto v:valuesMin)
        if(v<vMin) vMin = v;

    if (vMax==vMin)
    {
       vMax += vMax*0.1;
       vMin -= vMin*0.1;
    }
    else
    {
       vMax += (vMax-vMin)*0.1;
       vMin -= (vMax-vMin)*0.1;
    }
    yScale = (vMax-vMin)/10;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,vMin,vMax,yScale);
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,factorName+" ("+unit+")");

    dataEnd = timestamps.size()-1;
    if (dataEnd+1>showDataLen)
        dataStart = dataEnd-showDataLen+1;
    else
        dataStart = 0;

    long long dt = (timestamps.at(timestamps.size()-1)-timestamps.at(0));
    if (dt/showDataLen > 59*60*1000)
    {
        dataType = 3;
        xScale = 60*60*1000*(dt/(60*60*1000))/showDataLen ;
    }
    else if (dt/showDataLen > 59*1000)
    {
        dataType = 2;
        xScale = 60*1000*(dt/(60*1000))*10/showDataLen ;
    }
    else
    {
        dataType = 1;
        xScale = 60*1000*(dt/(60*1000))*5/showDataLen ;
    }

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,timestamps.at(dataStart),timestamps.at(dataEnd),xScale);

    curveVMax->setSamples(timestamps, valuesMax);
    curveVMax->attach(ui->qwtPlot);
    curveVMin->setSamples(timestamps, valuesMin);
    curveVMin->attach(ui->qwtPlot);
    curveVAvg->setSamples(timestamps, valuesAvg);
    curveVAvg->attach(ui->qwtPlot);

    ui->qwtPlot->replot();
}
