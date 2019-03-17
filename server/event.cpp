#include "event.h"
#include "file_codes.h"
Event::Event(QString name,MainNetworkManger *parent,QString e) : QObject(nullptr),ntwkmgr(parent),type(name),evid(e)
{
    connect(parent,&MainNetworkManger::Message,this,&Event::recv_t);
    connect(parent,&MainNetworkManger::dscnktd,this,&Event::dscnktd);
}

void Event::dscnktd()
{
    this->disconnect(ntwkmgr);
    this->ntwkmgr=nullptr;
}

void Event::reconnected()
{

}
void Event::window_info()
{
    this->senderr(-3);
}

bool Event::isreconnectedable()
{
    return false;
}

void Event::reconnected_t(MainNetworkManger * p)
{
    this->ntwkmgr=p;
    connect(p,&MainNetworkManger::Message,this,&Event::recv_t);
    connect(p,&MainNetworkManger::dscnktd,this,&Event::dscnktd);
    this->reconnected();
}

void Event::sendevt(const int &sid,const QVariantMap &args)
{
    ntwkmgr->sendevt(sid,evid,args);
}

void Event::senderr(const int &eid)
{
    ntwkmgr->senderr(eid,evid,File_Codes::read(eid));
}

void Event::recv_t(QVariantMap map)
{
    switch (map.value(JSON_ACT).toInt()){
        case 101:
        this->window_info();
        break;
    default:
        this->recv(map.value(JSON_ACT).toInt(),map.value(JSON_ARG).toMap());
    };
}

QString Event::GetType() const
{
    return type;
}
