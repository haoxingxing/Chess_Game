#include "eventmanger.h"
#include "random.h"
#include "file_codes.h"
#include "QDebug"
/*
 * id:
 *  1: login
 *  2: menu
 *  3: rank
 *  4: chess_place
 */


#include "login.h"
#include "menu.h"
#include "ranking.h"
EventManger::EventManger(MainNetworkManger *parent) : QObject(parent),ntwkmgr(parent)
{
    this->sockets.insert(ntwkmgr->getScid(),new Socket);
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
        if (sockets[ntwkmgr->getScid()]->events.contains(map.value(JSON_EVENT_ID).toString()))
        {
            sockets[ntwkmgr->getScid()]->events[map.value(JSON_EVENT_ID).toString()]->recv_t(map);
        } else {
            ntwkmgr->senderr(-2,"new",File_Codes::read(-2));
        }
    }
}

QList<Event*> EventManger::FindEvent(QString type)
{
    QList<Event*> list;
    foreach(Event* p,sockets.value(ntwkmgr->getScid())->events)
    {
        if (p->GetType()==type)
        {
            list.push_back(p);
        }
    }
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
    this->sockets.value(ntwkmgr->getScid())->events.insert(evid,pointer);
    ntwkmgr->sendnev(id,evid);
    return evid;
}

void EventManger::DelEvent(QString evid)
{
    delete this->sockets[ntwkmgr->getScid()]->events[evid];
    this->sockets[ntwkmgr->getScid()]->events.remove(evid);
}

Event *EventManger::GetEvent(QString evid)
{
    return this->sockets[ntwkmgr->getScid()]->events[evid];
}

void EventManger::disconnected()
{
    qDebug()<<"Disconnected Socket "<<ntwkmgr->getScid();
    qDebug()<<"Deleting unreconnectible events";
    foreach(Event* p,sockets.value(ntwkmgr->getScid())->events)
    {
        sockets.value(ntwkmgr->getScid())->events.remove(p->evid);
        if (!p->isreconnectedable())
        {
            delete p;
        }
    }
    if (islogged)
    {
        qDebug() << "Moving reconnectible events";
        foreach(Event* e,users[username][ntwkmgr->getScid()]->events)
        {
            users[username]["offlined"]->events[e->evid]=e;
            qDebug() << "Moved Event "<<e->evid;
        }
        users[username].remove(ntwkmgr->getScid());
    }
    else
    {
        qDebug() << "Deleting events";
        foreach(Event* p,sockets.value(ntwkmgr->getScid())->events)
        {
            delete p;
        }
    }
    sockets.remove(ntwkmgr->getScid());
    qDebug() << "Deleted ["<<this<<"]"<< ntwkmgr->getScid();
    ntwkmgr->deleteLater();
    this->deleteLater();
}

void EventManger::LoginStatusChanged()
{
    islogged=!islogged;
    if (islogged)
    {
        islogged=true;
        users[username][ntwkmgr->getScid()]=sockets[ntwkmgr->getScid()];
        if (users[username].contains("offlined"))
        {
            QMapIterator<QString,Event*> it(users[username]["offlined"]->events);
            while (it.hasNext())
            {
                it.next();
                users[username][ntwkmgr->getScid()]->events.insert(it.key(),it.value());
                it.value()->reconnected_t(ntwkmgr);
            }
            users[username].remove("offlined");
        }
    }
    else {
        qDebug() << "User Logged Out";        
        islogged=false;        
        users[username].remove(ntwkmgr->getScid());
        qDebug() << "Delete All Events";
        foreach(Event* p,sockets.value(ntwkmgr->getScid())->events)
        {
            delete p;
        }
        sockets.value(ntwkmgr->getScid())->events.clear();
        ntwkmgr->disconnect();        
    }
}
QMap<QString,EventManger::Socket*> EventManger::sockets=QMap<QString,EventManger::Socket*>();

QMap<QString,QMap<QString,EventManger::Socket*>> EventManger::users=QMap<QString,QMap<QString,EventManger::Socket*>>();
