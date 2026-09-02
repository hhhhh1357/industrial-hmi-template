#ifndef SUBWORKER3080_H
#define SUBWORKER3080_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker3080:public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker3080(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent = nullptr);
    //explicit SubWorker3084(QString ip,  QObject *parent = nullptr);

    ~SubWorker3080();

public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;



};

#endif // SUBWORKER3080_H
