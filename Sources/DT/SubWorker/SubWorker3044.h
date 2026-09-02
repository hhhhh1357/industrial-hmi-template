#ifndef SUBWORKER3044_H
#define SUBWORKER3044_H

#include <QObject>
#include "../SubWorkerBase.h"
#include "../PackingTools.h"
#include <QDebug>


class SubWorker3044:public SubWorkerBase
{
    Q_OBJECT
public:
    explicit SubWorker3044(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent = nullptr);
    //explicit SubWorker3044(QString ip,  QObject *parent = nullptr);

    ~SubWorker3044();

public slots:
    void doWork(Task task) ;
    QString buildCloudCmd(Task &task, bool &shouldDone) override;
public:
    bool needAnswer = false;



};


#endif // SUBWORKER3044_H
