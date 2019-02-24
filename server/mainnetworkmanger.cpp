#include "jsoncoder.h"
#include "mainnetworkmanger.h"
#include <QDebug>
#include <QThread>
#include "requestprocesser.h"
MainNetworkManger::MainNetworkManger(QObject *parent) : QObject(parent)
{
    server->listen(QHostAddress::Any,8864);
    QObject::connect(server,&QTcpServer::newConnection,this,&MainNetworkManger::newConnection);
}

void MainNetworkManger::newConnection()
{
    while(server->hasPendingConnections()){
        QTcpSocket *sock=server->nextPendingConnection();
        new RequestProcesser(nullptr,sock);
    }
}
