#ifndef SUBWORKER1062_H
#define SUBWORKER1062_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker1062 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker1062(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker1062();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER1062_H
