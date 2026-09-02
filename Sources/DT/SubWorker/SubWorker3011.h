#ifndef SUBWORKER3011_H
#define SUBWORKER3011_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker3011 : public SubWorkerBase
{
public:
    explicit SubWorker3011(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent=nullptr);

    ~SubWorker3011();


public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task,bool &shouldDone) override;
public:
    bool needAnswer = false;
};




#endif // SUBWORKER3011_H
