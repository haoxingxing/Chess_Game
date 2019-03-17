#include "event.h"
#include <QMessageBox>
Event::Event(MainNetworkManger* ntwkmgr,QString evid) : QObject(nullptr),ntwkmgr(ntwkmgr),evid(evid)
{
    connect(ntwkmgr,&MainNetworkManger::Disconnect,this,&Event::disconnected);
}

void Event::recv_t(int a, QVariantMap b)
{
    this->recv(a,b);
}

void Event::err(int eid, QString estr)
{
    QMessageBox::critical(nullptr,QString::number(eid),estr);
}

void Event::send(const int &act, const QVariantMap &args)
{
    ntwkmgr->sendevt(act,evid,args);
}
