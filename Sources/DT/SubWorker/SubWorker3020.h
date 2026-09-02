#ifndef SUBWORKER3020_H
#define SUBWORKER3020_H


#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker3020 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker3020(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker3020();

public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SubWorker3020_H

