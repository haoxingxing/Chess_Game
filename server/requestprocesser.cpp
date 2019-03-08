#include "jsoncoder.h"
#include "requestprocesser.h"
#include <QDebug>
#include "login.h"
#include "ranking.h"
#include "chess_gaming.h"
#include "chat.h"

RequestProcesser::RequestProcesser(QObject *parent,QTcpSocket* so) : QObject(parent)
{
    socket=so;
    connect(socket,&QTcpSocket::readyRead,this,&RequestProcesser::readyRead);
    connect(socket,&QTcpSocket::disconnected,this,&RequestProcesser::dscnktd);
    verify=new Login(this);
}

void RequestProcesser::send(QVariantMap str,QString _for)
{
    str.insert("for",_for);
    qDebug() << "server -> client " <<Jsoncoder::encode(str).toLatin1().toStdString().c_str();
    socket->write(Jsoncoder::encode(str).toUtf8() + "\r\n");
    socket->flush();
}


void RequestProcesser::readyRead()
{
    QString r=socket->readLine().simplified();
    qDebug()<< "client -> server "<<r.toStdString().c_str();
    QVariantMap map=Jsoncoder::deocde(r);
    if (map.value("for").toString()=="main") {
        if (verify->isLogin)
        {
            if (map.value("request").toString()=="rank")
                new ranking(this,map.value("numbers").toInt(),map.value("rank_info").toString());
            else if (map.value("request").toString()=="chess_place")
                new chess_gaming(this,map.value("x").toInt(),map.value("y").toInt());
            else if (map.value("request").toString()=="chat")
                new chat(this);
        }
    }
    else
        emit Message(map);
}
