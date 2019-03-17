#ifndef EVENTMANGER_H
#define EVENTMANGER_H

#include <QObject>
#include "event.h"
#include "server.h"
#define NE(whattime) NewEvent(File_Codes::readint(#whattime))

class Login;
class EventManger : public QObject
{
    Q_OBJECT
public:
    explicit EventManger(MainNetworkManger *ntwkmgr);
    QString username;
signals:

public slots:
    void recv(QVariantMap);
    QString NewEvent(int id);
    void DelEvent(QString evid);
    Event* GetEvent(QString evid);
    QList<Event*> FindEvent(QString type);
private:
    friend Login;
    struct Socket{
        QMap<QString/* Event ID */,Event*> events;
        bool isonline = true;
    };
    static QMap<QString /* Socket ID */,Socket*> sockets;
    static QMap<QString /* Username */,QMap<QString,Socket*>> users;
    void disconnected();
    MainNetworkManger *ntwkmgr;
    bool islogged = false;
    void LoginStatusChanged();
    void AnotherLogged();

};

#endif // EVENTMANGER_H
