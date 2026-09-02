#include "Headers/DAL/dal.h"

#include <QThread>
#include <QDebug>

Sqlite bSqlite;
QMutex mutexSqlite;

Sqlite::Sqlite():Sqlite("industrialhmi.db","industrialhmi","","")
{

}

Sqlite::Sqlite(QString m_file_name,QString m_host_name,QString m_user_name,QString m_pwd)
{
    db_file_name=m_file_name;
    if (db_file_name.isEmpty() || db_file_name.length()==0)
        db_file_name ="industrialhmi.db";

    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_file_name);
    if(!db.open())
    {
        qDebug()<<"database open error."<<endl;
        error = db.lastError();
    }

    query=new QSqlQuery(db);
}

Sqlite::Sqlite(QString m_file_name,QString m_user_name,QString m_pwd):Sqlite(m_file_name,"127.0.0.1",m_user_name,m_pwd)
{

}

Sqlite::~Sqlite()
{
   if (db.isOpen())  db.close();

   if (query!=NULL)
       delete query;
}

void Sqlite::closeDB()
{
    db.close();
    if (query!=NULL)
    {
        delete query;
        query=NULL;
    }
}

QSqlDatabase *Sqlite::getDatabasePtr()
{
    return & (this->db);
}

QSqlQuery *Sqlite::getQuery()
{
    return this->query;
}

 /****************create_table函数：创建数据库表***************
 * table_name:表名
 * table_data:表项 名->属性
 **********************************************************/
bool Sqlite::create_table(QString table_name,QMap<QString,QString> table_data)
{
   QString sql="create table '"+table_name+"' (";
   for(QMap<QString,QString>::const_iterator i=table_data.constBegin();i!=table_data.constEnd();i++)
   {
        sql+=i.key()+' '+i.value();
        if(i+1!=table_data.constEnd())
            sql+=", ";
   }
   sql+=")";

   QMutexLocker locker(&mutexSqlite);
   bool r = query->exec(sql);

   if (r==false)
   {
       error = query->lastError();
       QString err=error.text();
       err+="";
   }
   return r;
}

/****************create_index函数：创建索引表***************
* index_name:索引表名
* table_name:表名
* table_data:表项 名->属性（ASC/DESC）
**********************************************************/
bool Sqlite::create_index(QString index_name, QString table_name, QMap<QString, QString> table_data)
{
    QString sql="CREATE UNIQUE INDEX '"+index_name+"' ON '"+table_name+"' (";
    for(QMap<QString,QString>::const_iterator i=table_data.constBegin();i!=table_data.constEnd();i++)
    {
         sql+=i.key()+' '+i.value();
         if(i+1!=table_data.constEnd())
             sql+=", ";
    }
    sql+=")";
    QMutexLocker locker(&mutexSqlite);
    return query->exec(sql);
}

bool Sqlite::delete_table(QString table_name)
{
    QMutexLocker locker(&mutexSqlite);
    //(1)删除数据表
    bool r = query->exec("DROP TABLE '"+table_name+"'");

    //(2)删除相关的索引表
    r = query->exec("DROP INDEX 'Idx_"+table_name+"'");

    return true;
}

  /*
  * 向数据库中增加数据
  */

 bool Sqlite::add(QString table_name, QMap<QString, QString> data)
 {
      QString sql="insert into '"+table_name+ "' (";
      QString values=" values(";
      for(QMap<QString,QString>::const_iterator i=data.constBegin();i!=data.constEnd();i++)
      {
          sql+=i.key()+", ";
          values+=i.value()+", ";
      }
      sql.chop(2);
      values.chop(2);
      sql+=") ";
      values+=") ";
      sql+=values;
      QMutexLocker locker(&mutexSqlite);
      bool r = query->exec(sql);
      if (r==false)
      {
          error = query->lastError();
          QString err=error.text();
          err+="";
      }
      return r;
 }

  /*
   * 向数据库发送一条语句
   */

 int Sqlite::db_query(QString m_query_sql)
  {
     QMutexLocker locker(&mutexSqlite);
      query->prepare(m_query_sql);
      return query->exec();
  }

 /***



****/
bool Sqlite::isTableExist(QString tableName)
{
   QMutexLocker locker(&mutexSqlite);
   QString sql="SELECT count(*) FROM sqlite_master WHERE type='table' AND name='"+tableName+"'";
   query->prepare(sql);
   if (query->exec())
   {
       if (query->next()) {
           int s = query->value(0).toInt();
           if (s>0)
               return true;
           else
               return false;
       }
       else
          return false;
   }
   else
       return false;
}


int Sqlite::recordCount(QString tableName, QString where)
{
   QMutexLocker locker(&mutexSqlite);
   QString sql="SELECT count(*) FROM '"+tableName+"' WHERE "+where;
   if (query->exec(sql))
   {
       if (query->next()) {
           int s = query->value(0).toInt();
           return s;
       }
       else
          return -1;
   }
   else
       return -1;
}

bool Sqlite::execTransaction(QList<QString> &sqlList)
{
    QMutexLocker locker(& mutexSqlite);

    db.transaction();
    for (int i=0;i<sqlList.count();i++)
    {
        query->exec(sqlList.at(i));
    }
    bool r = db.commit();
    if (r==false)
    {
        error = query->lastError();
    }
    //QString err=error.text();
    return r;
}


  /*
   * 删除一条记录
   */

  int Sqlite::del(QString table_name, QMap<QString, QString> where)
  {
      QMutexLocker locker(&mutexSqlite);
      QString sql="delete from '";
      sql+=table_name;
      sql+="' where ";
      for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
      {
          sql+=i.key()+"=";
          sql+="'"+i.value()+"' ";
      }
      sql.chop(2);
      query->prepare(sql);
      return query->exec();
  }

  int Sqlite::del(QString tableName, QString where)
  {
      QString sql="delete from '";
      sql+=tableName;
      sql+="' where ";
      sql+=where;
      QMutexLocker locker(&mutexSqlite);
      return query->exec(sql);
  }

/*
  * 修改数据库记录
  */
bool Sqlite::update(QString table_name, QMap<QString, QString> where, QMap<QString, QString> data)
{
     QString sql="update '"+table_name+"' set ";
     for(QMap<QString,QString>::const_iterator i=data.constBegin();i!=data.constEnd();i++)
     {
         sql+=i.key()+"=";
         sql+=i.value();
         if (i+1!=data.constEnd())
             sql+=", ";
     }
     if (where.count()>0)
     {
         sql+=" where ";
         for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
         {
             sql+=i.key()+"=";
             sql+=i.value();
             if (i+1!=where.constEnd())
                 sql+=" and ";
         }
     }
//qDebug() << sql<<endl;
     bool r;
     {
         QMutexLocker locker(&mutexSqlite);
         r = query->exec(sql);
     }
     if (r==false)
     {
         error = query->lastError();
         QString err=error.text();
         qDebug()<<"SQL error:"<<err<<endl;
     }

     return r;
}

 /*Sqlite exec
  * 查找所有记录
  */


bool Sqlite::find(QString table_name,QList<QString> key,QMap<QString,QString> where,QList< QList<QString> > *row)
{
     QString sql="select ";
     int len=key.size();
     for(int i=0;i<len;i++)
     {
         sql+=key.at(i);
         sql+=",";
     }
     sql.chop(1);
     sql+=" from '"+table_name+"'";
     sql+=" where ";
     for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
     {
         sql+=i.key()+"=("+i.value()+") and ";
     }
     sql.chop(4);
     //qDebug()<<sql;
     QMutexLocker locker(&mutexSqlite);
     query->prepare(sql);
     if(query->exec())
     {
         while (query->next()) {
             QList<QString> j;
             for(int i=0;i<len;i++)
             {
                 j.append(query->value(i).toString());
             }
             row->append(j);
        }
         return true;
     }
     else return false;
}


bool Sqlite::find(QString table_name,QList<QString> key,QList< QList<QString> > *row)
{
     QString sql="select ";
     int len=key.size();
     for(int i=0;i<len;i++)
     {
         sql+=key.at(i);
         sql+=",";
     }
     sql.chop(1);
     sql+=" from '"+table_name +"'";
     //qDebug()<<sql;
     QMutexLocker locker(&mutexSqlite);
     query->prepare(sql);
     if(query->exec())
     {
         while (query->next())
         {
             QList<QString> j;
             for(int i=0;i<len;i++)
             {
                 j.append(query->value(i).toString());
                 //qDebug()<<query->value(i).toString()<<endl;
             }
             row->append(j);
         }
         return true;
     }
     else return false;
}

bool Sqlite::find(QString table_name, QList<QString> key, QMap<QString, QString> where, QString orderby, QList<QList<QString> > *row)
{
    QString sql="select ";
    int len=key.size();
    for(int i=0;i<len;i++)
    {
        sql+=key.at(i);
        sql+=",";
    }
    sql.chop(1);
    sql+=" from '"+table_name+"'";
    sql+=" where ";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"="+i.value()+",";
    }
    sql.chop(1);

    sql += " ORDER By "+ orderby;
    //qDebug()<<sql;
    QMutexLocker locker(&mutexSqlite);
    query->prepare(sql);
    bool r = query->exec();
    if(r)
    {
        while (query->next()) {
            QList<QString> j;
            for(int i=0;i<len;i++)
            {
                j.append(query->value(i).toString());
            }
            row->append(j);
       }
        return true;
    }
    else
    {
        error = query->lastError();
        QString err=error.text();
        return false;
    }
}

bool Sqlite::find(QString table_name,QString where,QList< QList<QString>> *row)
{
     QString sql="select * from '";
     sql+=table_name+"'";
     if (where.length()>0)
         sql+=" where "+where;

     QMutexLocker locker(&mutexSqlite);
     bool r = query->exec(sql);
     if (r)
     {
         while (query->next())
         {
             int len = query->record().count();
             QList<QString> j ;
             for(int i=0;i<len;i++)
             {
                 j.append(query->value(i).toString());
                 //qDebug()<<query->value(i).toString()<<endl;
             }
             row->append(j);
         }
         return true;
     }
     else
     {
         error = query->lastError();
         QString e = error.text();
         return false;
     }
}

bool Sqlite::find(QString sql, QList<QList<QString> > *row)
{
    QMutexLocker locker(&mutexSqlite);
    bool r = query->exec(sql);
    if (r)
    {
        while (query->next())
        {
            int len = query->record().count();
            QList<QString> j ;
            for(int i=0;i<len;i++)
            {
                j.append(query->value(i).toString());
            }
            row->append(j);
        }
        return true;
    }
    else
    {
        error = query->lastError();
        QString e = error.text();
        return false;
    }
}
 /*
  * 获取错误信息
  */

QString Sqlite::getError()
{
     return error.text();
 }

