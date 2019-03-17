#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <iostream>
class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = nullptr);

signals:

public slots:
private:
    QTcpSocket* socket;
};

#endif // CLIENT_H
