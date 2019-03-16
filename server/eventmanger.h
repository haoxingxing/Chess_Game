#ifndef EVENTMANGER_H
#define EVENTMANGER_H

#include <QObject>
#include "event.h"
#include "server.h"
class Login;
class EventManger : public QObject
{
    Q_OBJECT
public:
    explicit EventManger(MainNetworkManger *ntwkmgr);
signals:

public slots:
    void recv(QVariantMap);
private:
    friend Login;
    QList<Event*> FindEvent(QString type);
    void NewEvent(QString eid,Event*);
    struct Socket{
        QMap<QString/* Event ID */,Event*> events;
        bool isonline = true;
    };
    static QMap<QString /* Socket ID */,Socket*> sockets;
    static QMap<QString /* Username */,QMap<QString,Socket*>> users;
    void disconnected();
    MainNetworkManger *ntwkmgr;
    bool islogged = false;
    QString username;
    void LoginStatusChanged();
    void AnotherLogged();

};

#endif // EVENTMANGER_H
