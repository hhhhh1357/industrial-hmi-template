#ifndef SUBWORKERBASE_H
#define SUBWORKERBASE_H

#include "DataPacketData.h"

#include "Task.h"

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QMap>

#include "SqliteTools.h"

class SubWorkerBase : public QObject
{
    Q_OBJECT
public:
    //explicit SubWorkerBase(QString ip ,SqliteTools sqlTools, QObject *parent = nullptr);
    explicit SubWorkerBase(QString ip, QString pw, QString mn, SqliteTools *sqlTools, QObject *parent = nullptr);
    //explicit SubWorkerBase(QString ip , QObject *parent = nullptr);
    SubWorkerBase(const SubWorkerBase& swb);
    SubWorkerBase& operator=(const SubWorkerBase& obj);
    SubWorkerBase();

    virtual ~SubWorkerBase();

    SqliteTools *sqlTools;

public:

    QTimer timer;

    QString IP;   //表示来自哪个服务器
    QString QN1 ="";
    QString QN2 ="";
    QString ST = "";  //记录对话发起时的ST
    QString CN = "";  //记录对话发起时的CN
    QString PW = "";  //记录对话发起时的PW
    QString MN = "";  //记录对话发起时的MN
    int PNUM;     //需上传的总包数
    int PNO=0;      //表示当前包号

    int ReplySeqNo=0;      //ReplySeqNo表示第几次构建命令串

    QMap<QString, QString> packList;  //1、分包发送时，该列表包含每个发送包的CP内容（不包含应答之类的握手信息，是实际数据内容），建议每一条对应一个监测因子的数据；键值对：键为页号PNO(从1开始),值为CP中的实际污染因子对应的各指标值

    int Flag;     //应答
    int TimeOut=30;  //超时时间（秒）
    int RepeatAllowable=3;  //重发次数
    int CurrentRepeatNo = 0;   //当前重发了几次
    QDateTime LatestSendTimeStamp;   //最近发送数据包的时间戳

    QString LatestSendCmd;

    void startTimer();
    void stopTimer();

public slots:
    virtual void doWork(Task task);
    virtual QString buildCloudCmd(Task &task, bool &shouldDone);
    virtual bool validateCmd(QString cmd);

signals:
    void toPLC(Task task);
    void toCloud(QString msg);    //通知Worker中的槽函数sendToCloud，发送到云端
    void isDone(QString QN);                    //通知Worker中的槽函数，删除该SubWorker对象
public slots:
    void timerMethod();   //计时器到时，开始检查是否搬砖超时 >TimeOut    槽
};






#endif // SUBWORKERBASE_H
