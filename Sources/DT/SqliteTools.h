#ifndef SQLITETOOLS_H
#define SQLITETOOLS_H

#include <QObject>

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class SqliteTools : public QObject
{
    //Q_OBJECT
public:
    explicit SqliteTools(QString connectionName, QObject *parent = nullptr);

    //SqliteTools(QString connectionName);
    SqliteTools(const SqliteTools& obj);
    SqliteTools& operator=(const SqliteTools& obj);
    ~SqliteTools();

    bool isNewDialog(QString QN, QString ip);     //判断是否是新的对话记录
    bool isContinualDialog(QString QN, QString ip);    //判断是否是对话的继续
    bool newDialogRecord(QString QN, qlonglong subWorkerRef, int taskSource, QString ip);  //开始一条新的对话过程记录
    bool continueDialogRecord(QString QN1, QString QN2, QString ip);    //修改对话过程记录(根据QN1的值修改表记录中QN2这一列

    QString findQN1_By_QN2(QString QN2, QString ip);
    int findTaskSourceByQN(QString QN, QString ip);    //根据QN查找内存表记录对应的taskSource

    bool deleteDialogRecord(QString QN1, QString ip);    //删除对话记录
    //QObject* findObjectByQN(QString QN, QString ip);    //根据QN查找内存记录中的对象指针
    qlonglong findObjectByQN(QString QN, QString ip);    //根据QN查找内存记录中的对象指针

    bool updateFlag(QString QN, QString ip, int flag);

    bool deleteDeadWorker(int minutes);
private:
    QString connectionName;
    QSqlDatabase talkerHistoryDB;
    //QSqlQuery query;
    QSqlDatabase createMemoryDB();


};


//class SqliteTools2 : public QObject
//{
//    //Q_OBJECT
//public:
//    explicit SqliteTools2(QString connectionName, QObject *parent = nullptr);



//    ~SqliteTools2();


//private:
//    QString connectionName;
//    QSqlDatabase talkerHistoryDB;
//    QSqlQuery query;

//};


















#endif // SQLITETOOLS_H
