#ifndef FRAMEREALTIMEDATA_H
#define FRAMEREALTIMEDATA_H

#include <QFrame>
#include <QLabel>
#include <QTableWidget>
#include "framethermo.h"
#include "Headers/BLL/BDataItem.h"

namespace Ui {
class FrameRealTimeData;
}

class FrameRealTimeData : public QFrame
{
    Q_OBJECT

public:
    explicit FrameRealTimeData(QWidget *parent = nullptr);
    FrameRealTimeData(QList<MFactor *> * DataItemList, QList<MFactor *> * EnvinmentItemList, MFactor * wqs, QWidget *parent = nullptr);
    ~FrameRealTimeData();

public slots:
    void updateFactorDataItem();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_BtnDataSecurity_clicked();

private:
    Ui::FrameRealTimeData *ui;

    QList<MFactor *> * DataItemList;
    QList<FrameThermo*> * thermoList=nullptr;

    QList<MFactor *> * EnvironmentItemList;

    MFactor * wqs;

    //图
    int currPage=1,totalPages=0;
    int thermoTotals=0;
    int rowsPerPage = 2, colsPerPage = 3;

    //表
    int currPageTable=1,totalPagesTable=0,totalItemsTable=0,rowsPerPageTable=10;
    QLabel *tabW,*tabE;

    QTableWidgetItem statueItem[11][2];

    QTableWidgetItem envStatueItem[5][2];

    void refreshTable();

    void refreshTableStatus();

    void setComponent();

    void refreshComponent();

    bool eventFilter(QObject *obj, QEvent*event);

    QWidget * currWidget=nullptr;

};

#endif // FRAMEREALTIMEDATA_H
