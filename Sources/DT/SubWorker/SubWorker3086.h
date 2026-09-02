#ifndef SUBWORKER3086_H
#define SUBWORKER3086_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker3086:public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker3086(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent = nullptr);
    //explicit SubWorker3084(QString ip,  QObject *parent = nullptr);

    ~SubWorker3086();

public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;



};

#endif // SUBWORKER3086_H
