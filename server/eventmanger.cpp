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
EventManger::EventManger(MainNetworkManger *parent) : QObject(parent),ntwkmgr(parent)
{
    this->sockets.insert(ntwkmgr->getScid(),new Socket);
    connect(parent,&MainNetworkManger::Message,this,&EventManger::recv);
    connect(parent,&MainNetworkManger::dscnktd,this,&EventManger::disconnected);
    qDebug() << "Hello World ["<<this<<"]" << ntwkmgr->getScid();
}

void EventManger::recv(QVariantMap map)
{
    if (map.value("event").toString()=="new")
    {
        QString evid = Random::GetRandomString(EID_LEN);
        switch (map.value("id").toInt()) {
        case 1:
            NewEvent(evid,new Login(ntwkmgr,evid,this));
            ntwkmgr->sendnev(map.value("id").toInt(),evid);
            break;
        default:
            ntwkmgr->senderr(-1,"new",File_Codes::read(-1));
            break;
        }
    }
    else
    {
        if (sockets[ntwkmgr->getScid()]->events.contains(map.value("event").toString()))
        {
            sockets[ntwkmgr->getScid()]->events[map.value("event").toString()]->recv_t(map.value("arg").toMap());
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

void EventManger::NewEvent(QString eid,Event * pointer)
{
    this->sockets.value(ntwkmgr->getScid())->events.insert(eid,pointer);
    qDebug()<<"Created New Event: " <<eid<<"["<<pointer<<"] in Socket "<<ntwkmgr->getScid();
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
    qDebug() << "Good Bye The World ["<<this<<"]"<< ntwkmgr->getScid();
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
