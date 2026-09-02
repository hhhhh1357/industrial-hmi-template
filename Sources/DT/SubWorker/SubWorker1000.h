#ifndef SUBWORKER1000_H
#define SUBWORKER1000_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker1000 : public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker1000(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent = nullptr);

    ~SubWorker1000();
public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;

};



#endif // SUBWORKER1000_H
