#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include "mainnetworkmanger.h"
class Event
{

public:
    explicit Event(MainNetworkManger* ntwkmgr,QString evid);
    virtual ~Event(){}
    virtual void recv_t(int,QVariantMap);
    virtual void err(int eid,QString estr);
    virtual void recv(int status_id,QVariantMap args) = 0;
    virtual void disconnected();
    void send(const int& act, const QVariantMap& args);
    MainNetworkManger* ntwkmgr;

private:
    QString evid;
};

#endif // EVENT_H
