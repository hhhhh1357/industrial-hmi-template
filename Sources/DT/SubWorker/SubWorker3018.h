#ifndef SUBWORKER3018_H
#define SUBWORKER3018_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker3018 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker3018(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker3018();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER3018_H
