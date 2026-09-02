#ifndef CONNECTSTATUS_H
#define CONNECTSTATUS_H

#include <QObject>
//#include <QAbstractSocket>

class ConnectStatus //: public QObject
{
    //Q_OBJECT
public:
    //explicit ConnectStatus(QObject *parent = nullptr);

    QString ip;
    //QAbstractSocket::SocketState state;  //Socket状态
    bool network_state = true;
signals:

};

#endif // CONNECTSTATUS_H
