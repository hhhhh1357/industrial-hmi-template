#include "SqliteTools2.h"

#include <SubWorker/SubWorker2011.h>


SqliteTools2::SqliteTools2(QString connName,QObject *parent):QObject(parent)
{
    connectionName = connName;
    talkerHistoryDB = createMemoryDB();
}

SqliteTools2::SqliteTools2(const SqliteTools2 &obj)
{
    this->connectionName = obj.connectionName;
    this->talkerHistoryDB = obj.talkerHistoryDB;
}
SqliteTools2 &SqliteTools2::operator=(const SqliteTools2 &obj)
{
    this->connectionName = obj.connectionName;
    this->talkerHistoryDB = obj.talkerHistoryDB;
}

QSqlDatabase SqliteTools2::createMemoryDB()
{

    //添加数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", this->connectionName);
    //数据库连接命名
    db.setDatabaseName(":memory:");
    if(db.open()){
        query = QSqlQuery(db);
        query.exec("create table talkerHistory(QN1 varchar, QN2 varchar,  subWorkerRef blob, taskSource int default 0, ip varchar)");
    }
    return db;
    /*
    QSqlDatabase db;
    if(QSqlDatabase::contains(this->connectionName)){
        db = QSqlDatabase::database((this->connectionName));
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE", this->connectionName);
        db.setDatabaseName("TalkerHistoryDB.db");

    }
    return db;
    */
}
//判断是否是新的对话开始
bool SqliteTools2::isNewDialog(QString QN, QString ip)
{

    bool rs = false;

       // QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
        QString sql = "select count(*) from talkerHistory where (QN1=? or QN2=?) and ip=?  ";
        query.prepare(sql);
        query.addBindValue(QN);
        query.addBindValue(QN);
        query.addBindValue(ip);

        bool r = query.exec();


        query.next();
        int recordCount = query.value(0).toInt();
        rs=recordCount>0?false:true;
        qDebug()<<__FILE__<<","<<__func__<<", QN="<<QN <<", ip="<<ip<<", recordCount="<<recordCount<<endl;


    return rs;

}
//判断是否是前继对话
bool SqliteTools2::isContinualDialog(QString QN, QString ip)
{
    bool rs = true;
   // QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
    QString sql = "select * from talkerHistory where QN2=? and ip=?";
    query.prepare(sql);
    query.addBindValue(QN);
    query.addBindValue(ip);
    query.exec();

    rs=query.next()?true:false;
    return rs;
}
//开始新对话记录 //开始一条新的对话过程记录
bool SqliteTools2::newDialogRecord(QString QN, long long subWorkerRef, int taskSource, QString ip){
    bool rs = false;

       // QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
        QString sql = "insert into talkerHistory(QN1, subWorkerRef, taskSource, ip) values(?,?,?, ?)";
        query.prepare(sql);
        query.addBindValue(QN);
        query.addBindValue(subWorkerRef);
        query.addBindValue(taskSource);
        query.addBindValue(ip);

        rs = query.exec();

    return rs;
}
 //修改对话过程记录(根据QN1的值修改表记录中QN2这一列
bool SqliteTools2::continueDialogRecord(QString QN1, QString QN2, QString ip){
    bool rs = false;

       // QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
        QString sql = "update talkerHistory set QN2=? where QN1=? and ip=?";
        query.prepare(sql);
        query.addBindValue(QN2);
        query.addBindValue(QN1);
        query.addBindValue(ip);

        rs = query.exec();


    return rs;
}
//删除对话记录
bool SqliteTools2::deleteDialogRecord(QString QN, QString ip0)
{
    bool rs = true;

        //删除内存对象
        //QObject* obj = findObjectByQN(QN, ip);
        long long obj = findObjectByQN(QN, ip0);
        //if(obj!=nullptr){
        if(obj!=-1){

            bool rs = false;
            //删除内存记录
          //  QSqlQuery query=QSqlQuery(this->talkerHistoryDB);

            //统计删除操作开始前的记录数
            QString sql1 = "select count(*) from talkerHistory where QN1=? and ip=?";
            query.prepare(sql1);
            query.addBindValue(QN);
            query.addBindValue(ip0);

            if(query.exec() && query.next()){
                int c = query.value(0).toInt();
                qDebug()<<"before delete, total records="<<c<<endl;
            }

            QString sql = "delete from talkerHistory where QN1=? and ip=?";
            query.prepare(sql);
            query.addBindValue(QN);
            query.addBindValue(ip0);

            rs = query.exec();
            //统计删除操作完成后的记录数
            query.prepare(sql1);
            query.addBindValue(QN);
            query.addBindValue(ip0);
            if(query.exec() && query.next()){
                int c = query.value(0).toInt();
                qDebug()<<"after delete, total records="<<c<<endl;
            }
            //obj->deleteLater();
            //delete (SubWorker2011*)obj;

            qDebug()<<__FILE__<<","<<__func__<<", QN="<<QN <<", ip="<<ip0<<", rs="<<rs<<endl;
            return rs;
        }
        else
            return rs;




}
//根据QN2找到对应记录的QN1
QString SqliteTools2::findQN1_By_QN2(QString QN2, QString ip)
{

        QString QN1 = "";
       // QSqlQuery query=QSqlQuery(this->talkerHistoryDB);
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
//QObject* SqliteTools2::findObjectByQN(QString QN, QString ip){
long long SqliteTools2::findObjectByQN(QString QN, QString ip){
    //QObjet* objAddr=nullptr;
    long long objAddr=-1;

        //QSqlQuery query=QSqlQuery(this->talkerHistoryDB);


        QString sql = "select * from talkerHistory where (QN1=? or QN2=?) and ip=?";
        query.prepare(sql);
        query.addBindValue(QN);
        query.addBindValue(QN);
        query.addBindValue(ip);

        bool rs = query.exec();
        if(rs && query.next()){
            //objAddr = (QObject*)(query.value(2).toLongLong());
            objAddr = (query.value(2).toLongLong());

        }
        qDebug()<<__FILE__<<","<<__func__<<", QN="<<QN <<", ip="<<ip<<", rs = "<<rs <<endl;

    return objAddr;
}
//根据QN查找内存表记录对应的taskSource
int SqliteTools2::findTaskSourceByQN(QString QN, QString ip)
{
    int taskSource = 0;
    //QSqlQuery query = QSqlQuery(this->talkerHistoryDB);
    QString sql = "select * from talkerHistory where (QN1=? or QN2=?) and ip=?";
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
