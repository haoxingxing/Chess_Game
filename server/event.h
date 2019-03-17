#ifndef RQSTPRCS_H
#define RQSTPRCS_H

#include <QObject>
#include "mainnetworkmanger.h"
class EventManger;
class Event : public QObject
{
    Q_OBJECT
public:
    explicit Event(QString name,MainNetworkManger *parent,QString evid,EventManger* par);
    virtual void recv(const int&,const QVariantMap&) = 0;
    virtual void dscnktd();
    virtual void reconnected();
    virtual void window_info();
    virtual bool isreconnectedable();
    void hide();
    void show();
    void reconnected_t(MainNetworkManger*);
    void sendevt(const int &sid, const QVariantMap &args);
    void senderr(const int &eid);
    MainNetworkManger* ntwkmgr;
    QString GetType() const;
    EventManger* evtmgr;
    friend EventManger;
public slots:
    void recv_t(QVariantMap);
private:
    QString type;
    QString evid;
};

#endif // RQSTPRCS_H
