#ifndef FRAMEHISTORYDATA_H
#define FRAMEHISTORYDATA_H

#include "framediagram.h"

#include <QFrame>
#include <QStandardItemModel>

namespace Ui {
class FrameHistoryData;
}

class FrameHistoryData : public QFrame
{
    Q_OBJECT

public:
    explicit FrameHistoryData(QWidget *parent = nullptr);
    ~FrameHistoryData();

private slots:
    void on_btnQuery_clicked();

    void on_btnDown_clicked();

    void on_btnUp_clicked();

    void on_btnPageDown_clicked();

    void on_btnPageUp_clicked();

    void on_btnSaveSD_clicked();

    void on_btnSaveUSB_clicked();

private:
    Ui::FrameHistoryData *ui;

    FrameDiagram *frameDiagram=nullptr;

    QVector<double> values;
    QVector<double> valuesMax;
    QVector<double> valuesMin;
    QVector<double> valuesAvg;
    QVector<double> timestamps;

    int recCount=0;     //记录总数量
    int recPerPage=50;  //每页显示的数据条数
    int recStart=0;     //当前页第一条记录号
    int recEnd=0;       //当前页最后一条记录号

    MFactor factor;

    QString selectItem;    //选择的数据项目
    QString factorName;    //因子名称
    QString factorCode;    //因子编码
    QString dataPortName;  //数据端口名
    QString dataDeviceId;  //监测设备号


    bool queryData();         //查询数据
    void addDataToDiagram();  //显示数据到图表中

    void setPageButtonStatus(); //设置翻页按钮状态

    QStandardItemModel *model=nullptr;
    void fillTable(); //填充表格

    void exportData(QString driver);//导出数据
};

#endif // FRAMEHISTORYDATA_H
