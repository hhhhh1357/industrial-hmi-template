#ifndef SUBWORKER2031_H
#define SUBWORKER2031_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>
#include "../Worker.h"
#include <QStringBuilder>
#include <Headers/BLL/BLocalService.h>


class SubWorker2031: public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker2031(QString ip, QString pw, QString mn,SqliteTools *sqlTools, QObject *parent=nullptr);

    ~SubWorker2031();


public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task,bool &shouldDone) override;
public:
    bool needAnswer = false;
};

#endif // SUBWORKER2031_H
