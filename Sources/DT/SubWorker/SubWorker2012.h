#ifndef SUBWORKER2012_H
#define SUBWORKER2012_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker2012 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker2012(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent = nullptr);
    //explicit SubWorker2011(QString ip,  QObject *parent = nullptr);

    ~SubWorker2012();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;

};

#endif // SUBWORKER2012_H
