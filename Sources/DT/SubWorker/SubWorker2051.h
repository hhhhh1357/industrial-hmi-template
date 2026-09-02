#ifndef SUBWORKER2051_H
#define SUBWORKER2051_H


#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker2051 : public SubWorkerBase
{
public:
    explicit SubWorker2051(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent=nullptr);

    ~SubWorker2051();


public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task,bool &shouldDone) override;
public:
    bool needAnswer = false;
};



#endif // SUBWORKER2051_H
