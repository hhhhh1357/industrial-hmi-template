#ifndef SUBWORKER1064_H
#define SUBWORKER1064_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker1064 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker1064(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker1064();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER1064_H
