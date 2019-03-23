#include "event.h"
#include "file_codes.h"
#include "eventmanger.h"
Event::Event(QString name,MainNetworkManger *parent,QString e,EventManger* par) : QObject(nullptr),ntwkmgr(parent),evtmgr(par),type(name),evid(e)
{
    connect(parent,&MainNetworkManger::dscnktd,this,&Event::dscnktd);
    qDebug()<<"Created New Event: " <<evid<<"["<<this<<"] in Socket "<<ntwkmgr->getScid();
}

Event::~Event()
{
    qDebug() << "Deleted Event "<<evid<<"["<<this<<"]";
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
    QVariantMap m=File_Codes::readmap(type+"_wi");
    if (m.isEmpty())
        this->senderr(-3);
    else {
        this->sendevt(3,File_Codes::readmap(type+"_wi"));
    }
}

bool Event::isreconnectedable()
{
    return false;
}

void Event::hide()
{
    sendevt(3,QVariantMap({
                              std::make_pair("isshow",false)
                          }));
}

void Event::show()
{
    sendevt(3,QVariantMap({
                              std::make_pair("isshow",true)
                          }));
}

void Event::close()
{
    sendevt(3,QVariantMap());
    evtmgr->DelEvent(evid);
}

void Event::reconnected_t(MainNetworkManger * p)
{
    this->ntwkmgr=p;
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
