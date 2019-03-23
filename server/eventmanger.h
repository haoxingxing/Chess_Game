#ifndef EVENTMANGER_H
#define EVENTMANGER_H

#include <QEventLoop>
#include <QObject>
#include "event.h"
#include "server.h"
#include <QMap>

#define NE(whattime) NewEvent(File_Codes::readint(#whattime))

class Login;
class EventManger : public QObject
{
    Q_OBJECT
public:
    explicit EventManger(MainNetworkManger *ntwkmgr);
    QString username;
    enum event_types{
        none,
        login,
        menu,
        rank,
        chat,
        match
    };

public slots:
    void exec();
    void recv(QVariantMap);
    QString NewEvent(event_types id);
    void DelEvent(QString evid);
    Event* GetEvent(QString evid);
    QList<Event*> FindEvent(QString type);

private:
    friend Login;
    static QMap<QString /* Socket ID */,QMap<QString/* Event ID */,Event*>*> sockets;
    static QMap<QString /* Username */,QMap<QString,QMap<QString/* Event ID */,Event*>*>> users;
    void disconnected();
    MainNetworkManger *ntwkmgr;
    bool islogged = false;
    void LoginStatusChanged();
    void AnotherLogged();
    QEventLoop loop;

};

#endif // EVENTMANGER_H
