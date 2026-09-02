#ifndef SUBWORKER2011_H
#define SUBWORKER2011_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker2011:public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker2011(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent = nullptr);
    //explicit SubWorker2011(QString ip,  QObject *parent = nullptr);

    ~SubWorker2011();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;



};




#endif // SUBWORKER2011_H
