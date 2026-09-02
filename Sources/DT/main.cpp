#include "mainwindow.h"

#include <QApplication>
#include <QProcess>
#include <QDebug>
#include "plc.h"
#include "CloudServices.h"

#include "SqliteTools2.h"

PLC plc;

CloudServices *cs;//(ipAndPorts);

//SqliteTools2 *sqlTools;

int main(int argc, char *argv[])
{
    qDebug()<<__FILE__<<", "<<__func__<<"线程ID:"<<QThread::currentThread()<<endl;

    QApplication a(argc, argv);
    qRegisterMetaType<Task>("Task");
    qRegisterMetaType<PLCRequest>("Request");
    qRegisterMetaType<PLCResponse>("Response");
    qRegisterMetaType<DataPacketData>("DataPacketData");
    qRegisterMetaType<SubWorkerBase>("SubWorkerBase");

    //qRegisterMetaType<QMap<QString, QAbstractSocket::SocketState>>("QMap");
    qRegisterMetaType<ConnectStatus>("ConnectStatus");

    QString ipAndPorts[]={"171.34.197.243:5002:88880082086206:123456"};
    //QString ipAndPorts[]={"47.98.249.111:9411:88880082086206:123456"};
    CloudServices cs2(ipAndPorts);
    cs = &cs2;


    MainWindow w;
    w.show();
    return a.exec();
}
