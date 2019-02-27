#include "jsoncoder.h"
#include "requestprocesser.h"
#include <QDebug>
#include "login.h"
#include "ranking.h"
#include "chess_gaming.h"
RequestProcesser::RequestProcesser(QObject *parent,QTcpSocket* so) : QObject(parent)
{
    socket=so;
    connect(socket,&QTcpSocket::readyRead,this,&RequestProcesser::readyRead);
    connect(socket,&QTcpSocket::disconnected,this,&RequestProcesser::dscnktd);
    this->ToLogin();
}

void RequestProcesser::send(QString str,QString types)
{
    qDebug() << str.toLatin1().toStdString().c_str();
    socket->write(str.toLatin1() + "\r\n");
    socket->flush();
}

void RequestProcesser::ToLogin()
{
    send(Jsoncoder::encode(QVariantMap({
                                           std::make_pair("to","login"),
                                           std::make_pair("hide","network")
                                       })));
    ;
}

void RequestProcesser::SendLoginErrorMessage()
{
    send(Jsoncoder::encode(QVariantMap({
                                           std::make_pair("is_success",false),
                                           std::make_pair("emsg","Username or Password Error")
                                       })));
}

void RequestProcesser::SendLoginSuccessMessage()
{
    send(Jsoncoder::encode(QVariantMap({
                                           std::make_pair("to","menu"),
                                           std::make_pair("username",username),
                                           std::make_pair("close","login")
                                       })));
}

void RequestProcesser::Logout()
{
    isLogin=false;
    username.clear();
    send(Jsoncoder::encode(QVariantMap({
                                           std::make_pair("to","login"),
                                           std::make_pair("close","menu")
                                       })));
}

void RequestProcesser::LoginProcess(QVariantMap map,LoginType tp)
{
    if (isLogin)
        SendLoginSuccessMessage();
    else
    {
        if ( (tp==LOGIN)?Login::login(map.value("username").toString(),map.value("password").toString()):Login::_register(map.value("username").toString(),map.value("password").toString()))
        {
            isLogin=true;
            this->username=map.value("username").toString();
            SendLoginSuccessMessage();
        }
        else
            SendLoginErrorMessage();

    }
}

void RequestProcesser::readyRead()
{
    QString r=socket->readLine().simplified();
    qDebug()<<r.toStdString().c_str();
    QVariantMap map=Jsoncoder::deocde(r);
    if (map.value("for").toString()=="main") {
        if(map.value("type").toString()=="login")
            LoginProcess(map,LOGIN);
        else if (map.value("type").toString()=="register")
            LoginProcess(map,REGISTER);
        else if (map.value("type").toString()=="logout")
            Logout();
        else if (isLogin)
        {
            if (map.value("type").toString()=="rank")
                new ranking(nullptr,this,map.value("numbers").toInt(),username,map.value("rank_info").toString(),map.value("to").toString());
                //new ranking(nullptr,this,map.value("numbers").toInt(),username,map.value("rank_info").toString());
        }
    }
    else
        emit Message(map);
}
