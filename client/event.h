#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include "mainnetworkmanger.h"
class Event : public QObject
{
    Q_OBJECT
public:
    explicit Event(MainNetworkManger* ntwkmgr,QString evid);
    virtual void recv_t(int,QVariantMap);
    virtual void err(int eid,QString estr) = 0;
    virtual void recv(int status_id,QVariantMap args) = 0;
    virtual void disconnected() = 0;
    void send(const int& act, const QVariantMap& args);
signals:

private:
    MainNetworkManger* ntwkmgr;
    QString evid;
};

#endif // EVENT_H
