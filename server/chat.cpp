#include "chat.h"
#include "login.h"
chat::chat(RequestProcesser  *parent) : RQSTPRCS (CHAT_HEAD,parent)
{
    this->sendReady();
}


void chat::connectMessage(QString cn)
{

    if (!addrs.contains(cn))
        addrs.insert(cn,new chat_channel(cn));
    addrs[cn]->join(this);
    this->username=ntwkmgr->verify->username;
    this->sendJoined(cn);


}

void chat::disconnectMessage(QString cn)
{
    if (!isDeleted)
    {
    if (addrs.contains(cn))
        addrs[cn]->exit(this,username);
    this->sendExited(cn);
    this->dscnktd();
    }
}

chat::~chat()
{
}

void chat::recv(QVariantMap map)
{
    if (!isDeleted)
    switch (map.value("status").toInt()) {
    case 100:
        this->connectMessage(map.value("channel").toString());
        break;
    case 101:
        this->disconnectMessage(map.value("channel").toString());
        break;
    case 102:
        if (addrs.contains(map.value("tochannel").toString()))
            addrs[map.value("tochannel").toString()]->newMessage(ntwkmgr->verify->username,map.value("msg").toString());
    }
}

void chat::dscnktd()
{
    foreach(chat_channel* p,addrs)
    {
        p->exit(this,username);
    }
    isDeleted=true;
}

void chat::havenewChat(QString c,QString from, QString s)
{
    if (!isDeleted)
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",203),
                                  std::make_pair("channel",c),
                                  std::make_pair("from",from),
                                  std::make_pair("msg",s),
                              }),CHAT_HEAD);
}

void chat::sendJoined_(QString who, QString from)
{
    if (!isDeleted)
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",204),
                                  std::make_pair("channel",from),
                                  std::make_pair("who",who),
                              }),CHAT_HEAD);
}

void chat::sendExited_(QString who, QString from)
{
    if (!isDeleted)
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",205),
                                  std::make_pair("channel",from),
                                  std::make_pair("who",who),
                              }),CHAT_HEAD);
}

void chat::sendReady()
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",200)
                              }),CHAT_HEAD);
}

void chat::sendJoined(QString cn)
{
    if (!isDeleted)
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",201),
                                  std::make_pair("channel",cn),
                              }),CHAT_HEAD);
}

void chat::sendExited(QString cn)
{
    if (!isDeleted)
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",202),
                                  std::make_pair("channel",cn),
                              }),CHAT_HEAD);
}

QMap<QString,chat::chat_channel*> chat::addrs=QMap<QString,chat::chat_channel*>();

chat::chat_channel::chat_channel(QString name) : QObject (nullptr),this_n(name)
{

}

void chat::chat_channel::join(chat * addr)
{
    if (!all_members.contains(addr))
        all_members.append(addr);
    foreach(chat *p,all_members)
        p->sendJoined_(addr->ntwkmgr->verify->username,this_n);
}

void chat::chat_channel::exit(chat * addr,QString yn)
{
    all_members.removeAll(addr);
    foreach(chat *p,all_members)
        p->sendExited_(yn,this_n);
}

void chat::chat_channel::newMessage(QString from,QString s)
{
    foreach(chat *p,all_members)
        p->havenewChat(this_n,from,s);
}
