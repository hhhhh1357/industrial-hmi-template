#include "CloudDT.h"

CloudSocket::CloudSocket():QObject(){

}
CloudSocket::CloudSocket(const CloudSocket &cs){

}
CloudSocket& CloudSocket::operator=(const CloudSocket &cs){
    return *this;
}
void CloudSocket::stateChanged(QAbstractSocket::SocketState newValue){
    this->state = newValue;
}

CloudDT::CloudDT(QObject *parent) : QObject(parent)
{

}
void CloudDT::socketConfig(QString ipAndPort){
    QStringList list = ipAndPort.split(":");
    CloudSocket socket;
    socket.ip = list.at(0);
    socket.port = list.at(1).toUShort();
    socket.socket.connectToHost(QHostAddress(socket.ip), socket.port);

    //connect(&socket.socket, &QTcpSocket::connected, this, &CloudDT::connected);
    connect(&socket.socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), &socket.socket, SLOT(stateChanged(QAbstractSocket::SocketState)));
    this->socketList.append(socket);
}

void CloudDT::stateChanged(QAbstractSocket::SocketState newValue){

}
void CloudDT::receiveFromCloud(){

}
void CloudDT::sendToCloud(DataPacketData data){

}
