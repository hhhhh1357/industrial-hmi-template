#ifndef FRAMEDIAGRAM_H
#define FRAMEDIAGRAM_H

#include <QDateTime>
#include <QFrame>
#include <qwt_plot_curve.h>
#include <qwt_date_scale_draw.h>
#include <qwt_text.h>
#include <qwt_plot_grid.h>
#include <QTimer>
#include <QVector>
#include <qalgorithms.h>

#include <Headers/Model/MFactor.h>
#include <Headers/BLL/BHistoryData.h>

class TimeScakeDrawR: public QwtScaleDraw
{
public:
    TimeScakeDrawR(){ }
    virtual QwtText label(double v) const
    {
        QDateTime upTime;
        upTime=QDateTime::fromMSecsSinceEpoch((quint64)v);
        QString t = upTime.toString("MM-dd\nhh:mm:ss");
        return t;
    }
};

namespace Ui {
class FrameDiagram;
}

class FrameDiagram : public QFrame
{
    Q_OBJECT

public:
    explicit FrameDiagram(QWidget *parent = nullptr);
    FrameDiagram(MFactor * factor, QWidget *parent = nullptr);

    ~FrameDiagram();

    void setDiagramArgs(QString factorName, QString unit, QVector<double> values, QVector<double> timestamps,
                        double valueMax, double valueMin, double wValueMax, double wValueMin);

    void setDiagramArgs(QString factorName, QString unit, QVector<double> valuesMax, QVector<double> valuesMin,
                        QVector<double> valuesAvg, QVector<double> timestamps);


private:
    Ui::FrameDiagram *ui;

    TimeScakeDrawR * timeScale=nullptr;

    QwtPlotCurve * curve=nullptr;          //数据曲线
    QwtPlotCurve * curveMax=nullptr;      //数据告警最大值线
    QwtPlotCurve * curveMin=nullptr;      //数据告警最大值线

    QwtPlotCurve * curveVMax=nullptr;      //数据最大值线
    QwtPlotCurve * curveVMin=nullptr;      //数据最小值线
    QwtPlotCurve * curveVAvg=nullptr;      //数据平均值线

    QVector<double> values;        //纵轴 值
    QVector<double> timestamps;    //横轴 时间

    QVector<double> warningValueMax;
    QVector<double> warningValueMin;

    MFactor * factor;     //显示的要素

    QTimer * updateTimer=nullptr; //动态数据更新

    double xScale,yScale;
    int dataType=1; //1:实时数据； 2：分钟数据； 3：小时数据

    int showDataLen = 180;
    int dataStart,dataEnd;

    void setUpdateTimer(); //设置数据动态更新计时间器
};


#endif // FRAMEDIAGRAM_H
