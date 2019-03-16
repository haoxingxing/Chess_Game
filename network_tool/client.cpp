#include "client.h"

client::client(QObject *parent) : QObject(parent)
{
    socket=new QTcpSocket(nullptr);
    connect(socket,&QTcpSocket::readyRead,this,&client::recv);
    socket->connectToHost("127.0.0.1",8864);
    socket->waitForConnected();
    socket->write("{\"event\":\"new\",\"id\":1}");
    socket->flush();
}

void client::recv()
{
    std::cout<<qPrintable(socket->readAll());
    std::string in;
    std::cin>>in;
    socket->write((in+"\r\n").c_str());
}
