#ifndef SUBWORKER1012_H
#define SUBWORKER1012_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker1012 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker1012(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker1012();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};



#endif // SUBWORKER1012_H
