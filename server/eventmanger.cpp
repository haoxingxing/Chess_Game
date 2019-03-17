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
    NE(connected);
}

void EventManger::recv(QVariantMap map)
{
    if (map.value(JSON_EVENT_ID).toString()=="new")
    {
        NewEvent(map.value(JSON_NEW_EVENT_ID).toInt());
    }
    else
    {
        if (sockets[ntwkmgr->getScid()]->events.contains(map.value(JSON_EVENT_ID).toString()))
        {
            Event* p=sockets[ntwkmgr->getScid()]->events[map.value(JSON_EVENT_ID).toString()];
            p->recv_t(map.value(JSON_ARG).toMap());
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

QString EventManger::NewEvent(int id)
{
    QString evid = Random::GetRandomString(EID_LEN);
    Event* pointer;
    switch (id)
    {
    case 0:
        return "";
    case 1:
        pointer=new Login(ntwkmgr,evid,this);
        break;
    case 2:
        pointer=new menu(ntwkmgr,evid,this);
        break;
    case 3:
        pointer=new ranking(ntwkmgr,evid,this);
        break;
    default:
        ntwkmgr->senderr(-1,"new",File_Codes::read(-1));
        return "";
    }
    this->sockets.value(ntwkmgr->getScid())->events.insert(evid,pointer);
    ntwkmgr->sendnev(id,evid);
    qDebug()<<"Created New Event: " <<evid<<"["<<pointer<<"] in Socket "<<ntwkmgr->getScid();
    return evid;
}

void EventManger::DelEvent(QString evid)
{
    delete this->sockets[ntwkmgr->getScid()]->events[evid];
    this->sockets[ntwkmgr->getScid()]->events.remove(evid);
    qDebug() << "Deleted Event "<<evid;
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
            qDebug() << "Deleted Event "<<p->evid;
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
            qDebug() << "Deleted Event "<<p->evid;
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
    if (islogged)
    {
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
        users[username].remove(ntwkmgr->getScid());
    }
}
QMap<QString,EventManger::Socket*> EventManger::sockets=QMap<QString,EventManger::Socket*>();

QMap<QString,QMap<QString,EventManger::Socket*>> EventManger::users=QMap<QString,QMap<QString,EventManger::Socket*>>();
