#ifndef SUBWORKER3015_H
#define SUBWORKER3015_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker3015 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker3015(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker3015();

public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER3015_H
