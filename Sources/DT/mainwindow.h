#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataPacketData.h"
#include "Task.h"
#include "PackingTools.h"
#include "ConnectStatus.h"
#include <QAbstractSocket>
#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void sendToCloud(Task task);
    void requestCloudService(Task task);
private slots:
    void on_btnCon_clicked();

    void handleResult(QString msg);  //处理返回结果 槽
    void handleConnectionState(ConnectStatus connectionState);
    //void handleConnectionState();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
