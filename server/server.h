#ifndef MAINNETWORKMANGER_H
#define MAINNETWORKMANGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#define version "v2.0"

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
    void send(QString str);
public slots:
    void newConnection();
    void err();
private:
    void HandshakeHead(QTcpSocket*);
    QTcpServer *serverv = new QTcpServer;
};

#endif // MAINNETWORKMANGER_H
