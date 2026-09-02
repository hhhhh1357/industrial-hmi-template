#ifndef SUBWORKER2061_H
#define SUBWORKER2061_H


#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>

class SubWorker2061 : public SubWorkerBase
{
public:
    explicit SubWorker2061(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent=nullptr);

    ~SubWorker2061();


public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task,bool &shouldDone) override;
public:
    bool needAnswer = false;
};



#endif // SUBWORKER2061_H
