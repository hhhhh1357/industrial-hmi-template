#ifndef DAL
#define DAL
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMap>
#include <QInternal>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QVariantList>
#include <QMutex>

class Sqlite
{
public:
    QSqlDatabase db;
    QString db_file_name;   //数据库文件名
    QSqlError error;
    QSqlQuery *query;
    QSqlRecord rec;

    Sqlite();
    Sqlite(QString m_file_name,QString m_host_name,QString m_user_name,QString m_pwd);
    Sqlite(QString m_file_name,QString m_user_name,QString m_pwd);
    ~Sqlite();

    void closeDB();

    QSqlDatabase * getDatabasePtr();
    QSqlQuery * getQuery();

    bool create_table(QString table_name,QMap<QString,QString> table_data);   //新建表
    bool create_index(QString index_name,QString table_name,QMap<QString,QString> table_data);//新建索引
    bool delete_table(QString table_name);
    int db_query(QString m_query_sql); //发送数据库语句
    bool isTableExist(QString tableName);
    bool add(QString table_name,QMap<QString,QString> data); //增加数据

    int del(QString tableName, QString where);
    int del(QString table_name,QMap<QString,QString> where); //删除一条记录
    bool update(QString table_name,QMap<QString,QString> where,QMap<QString,QString> data);  //更新数据
    bool find(QString table_name,QList<QString> key,QMap<QString,QString> where,QList< QList<QString> > *row);    //查找
    bool find(QString table_name,QList<QString> key,QMap<QString,QString> where, QString orderby, QList< QList<QString> > *row);    //查找
    bool find(QString table_name,QList<QString> key,QList< QList<QString> > *row);    //查找所有
    bool find(QString table_name,QString where,QList< QList<QString> > *row);    //查找所有
    bool find(QString sql,QList< QList<QString> > *row);
    QString getError();
    int recordCount(QString tableName, QString where);

    bool execTransaction(QList<QString> &sqlList); //执行事务

};



#endif // DAL

