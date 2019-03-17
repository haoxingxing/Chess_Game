#include "jsoncoder.h"
#include "mainnetworkmanger.h"
#include <QDebug>
#include "random.h"

MainNetworkManger::MainNetworkManger(QObject *parent,QTcpSocket* so) : QObject(parent)
{
    socket=so;
    connect(socket,&QTcpSocket::readyRead,this,&MainNetworkManger::readyRead);
    connect(socket,&QTcpSocket::disconnected,this,&MainNetworkManger::dscnktd);
    this->scid=Random::GetRandomString(SID_LEN);
}

void MainNetworkManger::sendraw(const QVariantMap &args)
{
    qDebug() << "\033[33m" <<Jsoncoder::encode(args).toUtf8().toStdString().c_str() << "\033[0m";
    socket->write(Jsoncoder::encode(args).toUtf8() + "\r\n");
    socket->flush();
}

void MainNetworkManger::senderr(const int &eid,const QString &evid, const QString &err)
{
    QVariantMap map;
    map.insert(JSON_ERROR_ID,eid);
    map.insert(JSON_ERROR_STR,err);
    map.insert(JSON_EVENT_ID,evid);
    map.insert(JSON_MODE,2);
    this->sendraw(map);
}

void MainNetworkManger::sendevt(const int &sid,const QString &evid, const QVariantMap &args)
{
    QVariantMap map;
    map.insert(JSON_ACT,sid);
    map.insert(JSON_EVENT_ID,evid);
    map.insert(JSON_ARG,args);
    map.insert(JSON_MODE,3);
    this->sendraw(map);
}

void MainNetworkManger::sendnev(const int &id, const QString &evid)
{
    QVariantMap map;
    map.insert(JSON_EVENT_ID,evid);
    map.insert(JSON_NEW_EVENT_ID,id);
    map.insert(JSON_MODE,1);
    this->sendraw(map);
}

void MainNetworkManger::readyRead()
{
    QString r=socket->readLine().simplified();
    qDebug()<< "\033[32m"<<r.toStdString().c_str()<< "\033[0m";
    QVariantMap map=Jsoncoder::deocde(r);
    emit Message(map);
}

QString MainNetworkManger::getScid() const
{
    return scid;
}

