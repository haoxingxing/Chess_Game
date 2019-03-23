#include "eventmanger.h"
#include "random.h"
#include "file_codes.h"
#include "QDebug"
#include "login.h"
#include "menu.h"
#include "ranking.h"
EventManger::EventManger(MainNetworkManger *parent) : QObject(parent),ntwkmgr(parent)
{
    sockets[ntwkmgr->getScid()]=new QMap<QString,Event*>;
    connect(parent,&MainNetworkManger::Message,this,&EventManger::recv);
    connect(parent,&MainNetworkManger::dscnktd,this,&EventManger::disconnected);
    qDebug() << "New ["<<this<<"]" << ntwkmgr->getScid();
}

void EventManger::exec()
{
    Login* l=dynamic_cast<Login*>(GetEvent(NewEvent(login)));
    l->exec();
    LoginStatusChanged();
    if (islogged)
    {
        NewEvent(menu);
    }
    l->exec();
    LoginStatusChanged();
}

void EventManger::recv(QVariantMap map)
{
    if (map.value(JSON_EVENT_ID).toString()=="new")
    {
#if 0
        NewEvent(map.value(JSON_NEW_EVENT_ID).toInt());
#else
        ntwkmgr->senderr(-6,"new",File_Codes::read(-2));
#endif
        /*
         * Disabled Client To Request for Event
         */
    }
    else
    {
        if (sockets[ntwkmgr->getScid()]->contains(map.value(JSON_EVENT_ID).toString()))
        {
            (*sockets[ntwkmgr->getScid()])[map.value(JSON_EVENT_ID).toString()]->recv_t(map);
        } else {
            ntwkmgr->senderr(-2,"new",File_Codes::read(-2));
        }
    }
}

QList<Event*> EventManger::FindEvent(QString type)
{
    QList<Event*> list;
    foreach(Event* p,*sockets[ntwkmgr->getScid()])    
        if (p->GetType()==type)        
            list.push_back(p);            
    return list;
}

QString EventManger::NewEvent(EventManger::event_types id)
{
    QString evid = Random::GetRandomString(EID_LEN);
    Event* pointer;
    switch (id)
    {
    case none:
        return "";
    case login:
        pointer=new Login(ntwkmgr,evid,this);
        break;
    case menu:
        pointer=new Menu(ntwkmgr,evid,this);
        break;
    case rank:
        pointer=new ranking(ntwkmgr,evid,this);
        break;
    default:
        ntwkmgr->senderr(-1,"new",File_Codes::read(-1));
        return "";
    }
    this->sockets.value(ntwkmgr->getScid())->insert(evid,pointer);
    ntwkmgr->sendnev(id,evid);
    return evid;
}

void EventManger::DelEvent(QString evid)
{
    delete (*this->sockets[ntwkmgr->getScid()])[evid];
    this->sockets[ntwkmgr->getScid()]->remove(evid);
}

Event *EventManger::GetEvent(QString evid)
{
    return (*this->sockets[ntwkmgr->getScid()])[evid];
}

void EventManger::disconnected()
{
    qDebug()<<"Disconnected Socket "<<ntwkmgr->getScid();
    if (islogged)
    {
        
        qDebug() << "Moving reconnectible events";
        foreach(Event* e,*(users[username])[ntwkmgr->getScid()])
        {
            if (e->isreconnectedable())
            {
                (*users[username]["offlined"])[e->evid]=e;
                users[username][ntwkmgr->getScid()]->remove(e->evid);
                qDebug() << "Moved Event "<<e->evid;
            }
        }
        qDebug()<<"Deleting unreconnectible events";
        foreach(Event* p,*sockets[ntwkmgr->getScid()])
        {
            sockets[ntwkmgr->getScid()]->remove(p->evid);
            delete p;
        }
        users[username].remove(ntwkmgr->getScid());
    }
    else
    {
        qDebug() << "Deleting events";
        foreach(Event* p,*sockets[ntwkmgr->getScid()])
        {
            sockets[ntwkmgr->getScid()]->remove(p->evid);
            delete p;
        }
        delete sockets[ntwkmgr->getScid()];
    }
    sockets.remove(ntwkmgr->getScid());
    qDebug() << "Deleted ["<<this<<"]"<< ntwkmgr->getScid();
    delete ntwkmgr;
    delete this;
}

void EventManger::LoginStatusChanged()
{
    islogged=!islogged;
    if (islogged)
    {
        users[username][ntwkmgr->getScid()]=sockets[ntwkmgr->getScid()];
        if (users[username].contains("offlined"))
        {
            QMapIterator<QString,Event*> it(*users[username]["offlined"]);
            while (it.hasNext())
            {
                it.next();
                users[username][ntwkmgr->getScid()]->insert(it.key(),it.value());
                it.value()->reconnected_t(ntwkmgr);
            }            
            delete users[username]["offlined"];
            users[username].remove("offlined");
        }
    }
    else {
        qDebug() << "User Logged Out";        
        qDebug() << "Delete All Events";
        foreach(Event* p,*sockets[ntwkmgr->getScid()])        
        {
            sockets[ntwkmgr->getScid()]->remove(p->evid);
            delete p;        
        }
        users[username].remove(ntwkmgr->getScid());
        sockets.value(ntwkmgr->getScid())->clear();    
        ntwkmgr->disconnect();        
    }
}
QMap<QString,QMap<QString/* Event ID */,Event*>*> EventManger::sockets=QMap<QString,QMap<QString/* Event ID */,Event*>*>();

QMap<QString,QMap<QString,QMap<QString/* Event ID */,Event*>*>> EventManger::users=QMap<QString,QMap<QString,QMap<QString/* Event ID */,Event*>*>>();
