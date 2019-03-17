#include "client.h"

client::client(QObject *parent) : QObject(parent)
{
    socket=new QTcpSocket(nullptr);
    socket->connectToHost("127.0.0.1",8864);
    socket->waitForConnected();
    socket->write("{\"event\":\"new\",\"id\":1}");
    socket->flush();
    while (true)
    {
        std::string in;
        std::cin>>in;
        socket->write((in+"\r\n").c_str());
    }
}
