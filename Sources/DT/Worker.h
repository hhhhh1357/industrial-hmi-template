#ifndef QA_H
#define QA_H

#include "DataPacketData.h"
#include "SubWorkerBase.h"
#include "Task.h"

#include <QMap>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "SqliteTools.h"
#include "QDebug"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QString serverIP, QString serverPort, QString pw, QString mn, QObject *parent = nullptr);

    //维护一个内存表的工具对象
    SqliteTools *sqlTools = nullptr;
    ~ Worker();


signals:

    void toCloud(QString msg);    //向上发给CloudTalker的信号，
    void dispatchTask(Task task);   //向下派发任务给SubWorker的信号，由SubWorker的buildCloudCmd处理
public slots:
    void doWork(Task task);  //分配干活
    void handleToCloud(QString msg);     //接收SubWorker的信号  槽

    void destroy(QString QN);  //接收SubWorker的信号isDone，槽===>删除指针对象和内存记录

    void deleteDeadWorker(int afterMinutes);
private:
    QString IP;   //记录该工人是为哪个云端干活
    QString PORT;
    QString PW;   //记录密码
    QString MN;   //记录设备
};





#endif // QA_H
