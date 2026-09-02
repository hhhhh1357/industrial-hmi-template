#ifndef SUBWORKER1011_H
#define SUBWORKER1011_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker1011 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker1011(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker1011();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};


#endif // SUBWORKER1011_H
