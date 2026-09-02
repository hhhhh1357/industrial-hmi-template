#include "SqliteTools.h"

#include "SubWorker/SubWorker2011.h"

#include <QSqlError>

//SqliteTools2::SqliteTools2(QString connectionName, QObject *parent)
//{

//}

//SqliteTools2::~SqliteTools2()
//{

//}

SqliteTools::SqliteTools(QString connName,QObject *parent):QObject(parent)
{
    connectionName = connName;
    //qDebug()<<__FILE__<<","<<__func__<<", before call createMemoryDB()"<<endl;

    talkerHistoryDB = createMemoryDB();
}

SqliteTools::SqliteTools(const SqliteTools &obj)
{
    this->connectionName = obj.connectionName;
    //qDebug()<<__FILE__<<","<<__func__<<" *****"<<endl;
    this->talkerHistoryDB = obj.talkerHistoryDB;
}
SqliteTools &SqliteTools::operator=(const SqliteTools &obj)
{
    this->connectionName = obj.connectionName;
    //qDebug()<<__FILE__<<","<<__func__<<"###########"<<endl;
    this->talkerHistoryDB = obj.talkerHistoryDB;
}
SqliteTools::~SqliteTools()
{
    //qDebug()<<__FILE__<<","<<__func__<<"-----------"<<endl;
}

QSqlDatabase SqliteTools::createMemoryDB()
{
    //qDebug()<<__FILE__<<","<<__func__<<", connectionName="<<this->connectionName<<endl;
    //添加数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", this->connectionName);
    //数据库连接命名
    db.setDatabaseName(":memory:");

    if(db.open()){
        QSqlQuery query = QSqlQuery(db);
        query.exec("create table talkerHistory(QN1 varchar, QN2 varchar,  subWorkerRef blob, taskSource int default 0, ip varchar,  flag int default 0, latestDT DateTime)");
    }
    return db;

    /*

    QSqlDatabase db;
    if(QSqlDatabase::contains(this->connectionName)){
        db = QSqlDatabase::database((this->connectionName));

    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", this->connectionName);
        db.setDatabaseName("TalkerHistoryDB");

    }
    query = QSqlQuery(db);
    bool rs = false;
    rs = db.open();
    return db;
    */
}
//判断是否是新的对话开始
bool SqliteTools::isNewDialog(QString QN, QString ip)
{

    bool rs = false;
    QSqlQuery query = QSqlQuery(this->talkerHistoryDB);
    //QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
    QString sql = "select count(QN1) from talkerHistory where (QN1=? or QN2=?) and ip=?  and flag=0";
    //QString sql = "select count(*) from talkerHistory where (QN1='"+QN+"'  or QN2='"+QN+"') and ip='"+ip+"' and flag=0";
    query.prepare(sql);
    query.addBindValue(QN);
    query.addBindValue(QN);
    query.addBindValue(ip);

    bool r = query.exec();
    if (r==false)
    {
        QSqlError error = query.lastError();
        QString err = error.text();
        err="";
    }

    query.next();
    int recordCount = query.value(0).toInt();
    rs=recordCount>0?false:true;
    //qDebug()<<__FILE__<<","<<__func__<<", QN="<<QN <<", ip="<<ip<<", recordCount="<<recordCount<<endl;


    return rs;

}
//判断是否是前继对话
bool SqliteTools::isContinualDialog(QString QN, QString ip)
{
    bool rs = true;
    QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
    QString sql = "select * from talkerHistory where QN2=? and ip=?";
    query.prepare(sql);
    query.addBindValue(QN);
    query.addBindValue(ip);
    query.exec();

    rs=query.next()?true:false;
    return rs;
}
//开始新对话记录 //开始一条新的对话过程记录
bool SqliteTools::newDialogRecord(QString QN, qlonglong subWorkerRef, int taskSource, QString ip){
    bool rs = false;

        QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
        QString sql = "insert into talkerHistory(QN1, subWorkerRef, taskSource, ip, latestDT) values(?,?,?,?,?)";
        query.prepare(sql);
        query.addBindValue(QN);
        query.addBindValue(subWorkerRef);
        query.addBindValue(taskSource);
        query.addBindValue(ip);

        QDateTime curDT = QDateTime::currentDateTime();
        query.addBindValue(curDT);
        rs = query.exec();

    return rs;
}
 //修改对话过程记录(根据QN1的值修改表记录中QN2这一列
bool SqliteTools::continueDialogRecord(QString QN1, QString QN2, QString ip){
    bool rs = false;

        QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
        QString sql = "update talkerHistory set QN2=?, latestDT=? where QN1=? and ip=?";
        query.prepare(sql);
        query.addBindValue(QN2);
        QDateTime curDT = QDateTime::currentDateTime();
        query.addBindValue(curDT);

        query.addBindValue(QN1);
        query.addBindValue(ip);

        rs = query.exec();


    return rs;
}
//删除对话记录
bool SqliteTools::deleteDialogRecord(QString QN, QString ip)
{
    bool rs = true;
    QSqlQuery query = QSqlQuery(this->talkerHistoryDB);
        //删除内存对象
        //QObject* obj = findObjectByQN(QN, ip);
        qlonglong obj = findObjectByQN(QN, ip);
        //if(obj!=nullptr){
        if(obj!=-1){

            bool rs = false;

            QString sql = "delete from talkerHistory where QN1=? and ip=?";
            query.prepare(sql);
            query.addBindValue(QN);
            query.addBindValue(ip);

            rs = query.exec();

            ((QObject*)obj)->deleteLater();
            //qDebug()<<__FILE__<<","<<__func__<<", QN="<<QN <<", ip="<<ip<<", rs="<<rs<<endl;
            return rs;
        }
        else
            return rs;
}
//根据QN2找到对应记录的QN1
QString SqliteTools::findQN1_By_QN2(QString QN2, QString ip)
{

        QString QN1 = "";
        QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
        QString sql = "select * from talkerHistory where QN2=? and ip=?";
        query.prepare(sql);
        query.addBindValue(QN2);
        query.addBindValue(ip);

        bool rs = query.exec();
        if(rs && query.next()){
            QN1 = query.value(0).toString();
        }
        return QN1;


}
//根据QN查找内存记录中的对象指针
//QObject* SqliteTools::findObjectByQN(QString QN, QString ip){
qlonglong SqliteTools::findObjectByQN(QString QN, QString ip){
    //QObjet* objAddr=nullptr;
    QSqlQuery query = QSqlQuery(this->talkerHistoryDB);
    qlonglong objAddr=-1;
        QString sql = "select * from talkerHistory where (QN1=? or QN2=?) and ip=? and flag=0";
        query.prepare(sql);
        query.addBindValue(QN);
        query.addBindValue(QN);
        query.addBindValue(ip);

        bool rs = query.exec();
        if(rs && query.next()){
            //objAddr = (QObject*)(query.value(2).toLongLong());
            objAddr = (query.value(2).toLongLong());

        }
        //qDebug()<<__FILE__<<","<<__func__<<", QN="<<QN <<", ip="<<ip<<", rs = "<<rs <<endl;

    return objAddr;
}
//根据QN查找内存表记录对应的taskSource
int SqliteTools::findTaskSourceByQN(QString QN, QString ip)
{
    int taskSource = 0;
    QSqlQuery query = QSqlQuery(this->talkerHistoryDB);
    QString sql = "select * from talkerHistory where (QN1=? or QN2=?) and ip=? and flag=0";
    query.prepare(sql);
    query.addBindValue(QN);
    query.addBindValue(QN);
    query.addBindValue(ip);

    bool rs = query.exec();
    if(rs && query.next()){
        taskSource = query.value(3).toInt();
    }
    return taskSource;
}
bool SqliteTools::updateFlag(QString QN, QString ip, int flag)
{
    QString sql = "update talkerHistory set flag=? where (QN1=? or QN2=?) and ip=? and flag=0";
    QSqlQuery query = QSqlQuery(this->talkerHistoryDB);
    query.prepare(sql);
    query.addBindValue(1);
    query.addBindValue(QN);
    query.addBindValue(QN);
    query.addBindValue(ip);

    bool rs = query.exec();
    return rs;
}

bool SqliteTools::deleteDeadWorker(int minutes)
{
    QString sql = QString("SELECT * from talkerHistory where (strftime('%s','now') - strftime('%s',latestDT))>%1").arg(minutes*60);
    QSqlQuery qry = QSqlQuery(this->talkerHistoryDB);
    qry.prepare(sql);
    bool rs =qry.exec();
    /*create table talkerHistory(QN1 varchar, QN2 varchar,
    subWorkerRef blob, taskSource int default 0, ip varchar,  flag int default 0, latestDT DateTime)*/
    if(rs){
        while(qry.next()){
            QString QN1 = qry.value("QN1").toString();
            QString QN2 = qry.value("QN2").toString();
            qlonglong objRef = qry.value("subWorkerRef").toLongLong();
            QString ip = qry.value("ip").toString();
            deleteDialogRecord(QN1, ip);
        }
        qry.clear();
    }
    return true;
}
