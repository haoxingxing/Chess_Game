#ifndef MAINNETWORKMANGER_H
#define MAINNETWORKMANGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MainNetworkManger : public QObject
{
    Q_OBJECT
public:
    explicit MainNetworkManger(QObject *parent = nullptr);
    void send(QString str);
public slots:
    void newConnection();
private:
    QTcpServer *server = new QTcpServer;
};

#endif // MAINNETWORKMANGER_H
