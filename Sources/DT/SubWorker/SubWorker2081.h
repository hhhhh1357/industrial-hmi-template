#ifndef SUBWORKER2081_H
#define SUBWORKER2081_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker2081: public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker2081(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent=nullptr);

    ~SubWorker2081();


public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task,bool &shouldDone) override;
public:
    bool needAnswer = false;
};
#endif // SUBWORKER2081_H
