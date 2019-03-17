#include "event.h"
#include <QMessageBox>
Event::Event(MainNetworkManger* ntwkmgr,QString evid) : ntwkmgr(ntwkmgr),evid(evid)
{
}

void Event::recv_t(int a, QVariantMap b)
{
    this->recv(a,b);
}

void Event::err(int eid, QString estr)
{
    QMessageBox::critical(nullptr,QString::number(eid),estr);
}

void Event::disconnected()
{
    delete this;
}

void Event::send(const int &act, const QVariantMap &args)
{
    ntwkmgr->sendevt(act,evid,args);
}
