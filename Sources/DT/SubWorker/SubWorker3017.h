#ifndef SUBWORKER3017_H
#define SUBWORKER3017_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker3017 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker3017(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker3017();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};
#endif // SUBWORKER3017_H
