#ifndef SUBWORKER2062_H
#define SUBWORKER2062_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker2062 : public SubWorkerBase
{
public:
    explicit SubWorker2062(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent=nullptr);

    ~SubWorker2062();


public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task,bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER2062_H
