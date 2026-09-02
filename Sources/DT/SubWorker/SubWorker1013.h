#ifndef SUBWORKER1013_H
#define SUBWORKER1013_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker1013 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker1013(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker1013();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER1013_H
