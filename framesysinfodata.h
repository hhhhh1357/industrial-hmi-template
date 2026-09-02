#ifndef FRAMESYSINFODATA_H
#define FRAMESYSINFODATA_H

#include <QFrame>
#include <QTableWidget>

#include <Sources/DT/Task.h>

namespace Ui {
class FrameSysInfoData;
}

class FrameSysInfoData : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSysInfoData(QWidget *parent = nullptr);
    ~FrameSysInfoData();

signals:
    bool requestPLC(Task task); //向 PLC 发请求

private slots:
    void on_btnWP_clicked();

private:
    Ui::FrameSysInfoData *ui;

    QTableWidgetItem factorItem[20][2];
    QTableWidgetItem configItem[20];

    void listFactors();
    void listConfig();

    QTimer * refreshWarningTimer=nullptr;// 刷新告警信息的定时器对象
    void setRefreshWarningTimer();

    int warningCount=0;
    void getWarningInfo();
};

#endif // FRAMESYSINFODATA_H
