#include "jsoncoder.h"
#include "login.h"
#include "eventmanger.h"
#include <QCryptographicHash>

#include "file_codes.h"
Login::Login(MainNetworkManger *rp,QString evid,EventManger*p):Event(LOGIN_HEAD,rp,evid,p)
{
}

void Login::recv(const int& status,const QVariantMap& map)
{
    switch(status)
    {
    case 201:
        this->login(map.value("username").toString(),map.value("password").toString());
        break;
    case 202:
        this->_register(map.value("username").toString(),map.value("password").toString());
        break;
    case 203:
        this->Logout();
        return;
    }
}

bool Login::login(QString username, QString password)
{
    if (readfromfile().value(username).toString()==QString("%1").arg(QString(QCryptographicHash::hash((password+"Server_Sencond_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex())))
    {
        this->username=username;
        isLogin=true;
        changeStatus();
        sendevt(1,QVariantMap({
                std::make_pair("username",username)
                              }));
        hide();
        this->next_step();
        return true;
    }
    else
    {
        senderr(-4);
        return false;
    }
}

bool Login::_register(QString username, QString password)
{
    QVariantMap bmap = readfromfile();
    if (bmap.contains(username))
    {
        senderr(-5);
        return false;
    }
    else
    {
        bmap.insert(username,QString("%1").arg(QString(QCryptographicHash::hash((password+"Server_Sencond_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex())));
        writetofile(bmap);
        isLogin=true;
        this->username=username;
        changeStatus();
        sendevt(1,QVariantMap({
                std::make_pair("username",username)
                              }));
        hide();
        this->next_step();
        return true;
    }
}

void Login::writetofile(QVariantMap vm)
{
    QFile F(SAVE_AS);
    F.open(QIODevice::ReadWrite);
    F.resize(0);
    F.write(Jsoncoder::encode(vm).toUtf8());
    F.flush();
    F.close();
}

QVariantMap Login::readfromfile()
{
    QFile F(SAVE_AS);
    F.open(QIODevice::ReadWrite);
    return Jsoncoder::deocde(F.readAll());
}

void Login::Logout()
{
    isLogin=false;
    changeStatus();
    username.clear();
    sendevt(2,QVariantMap({}));
    show();
}

void Login::changeStatus()
{
    evtmgr->islogged=isLogin;
    evtmgr->username=username;
    evtmgr->LoginStatusChanged();
}

void Login::next_step()
{
    evtmgr->NewEvent(2);
}
