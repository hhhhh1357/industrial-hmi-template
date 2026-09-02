#ifndef SUBWORKER1063_H
#define SUBWORKER1063_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker1063 : public SubWorkerBase
{

    Q_OBJECT
public:
    explicit SubWorker1063(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker1063();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER1063_H
