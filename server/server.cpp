#include "jsoncoder.h"
#include "server.h"
#include <QDebug>
#include <QThread>
#include "eventmanger.h"
#include "mainnetworkmanger.h"
server::server(QObject *parent) : QObject(parent)
{
    serverv->listen(QHostAddress::Any,8864);
    QObject::connect(serverv,&QTcpServer::newConnection,this,&server::newConnection);
    connect(serverv,&QTcpServer::acceptError,this,&server::err);
}

void server::newConnection()
{
    while(serverv->hasPendingConnections()){
        QTcpSocket *sock=serverv->nextPendingConnection();
        new EventManger(new MainNetworkManger(this,sock));
    }
}

void server::err()
{
    qDebug() << serverv->errorString();
}

void server::HandshakeHead(QTcpSocket * socket)
{
    socket->write("HEAD " version "\r\n");
    socket->flush();
}
