#include "jsoncoder.h"
#include "login.h"

#include <QCryptographicHash>


Login::Login(RequestProcesser *rp):RQSTPRCS("login",rp)
{

}

/* Some codes:
 * recv:
 *      301: login
 *      302: register
 *      303: logout
 * send:
 *      301: lr error
 *      302: lr success
 *      303: logout successful
 *
*/
void Login::recv(QVariantMap map)
{
    QString emsg;
    switch(map.value("type").toInt())
    {
    case 301:
        this->login(map.value("username").toString(),map.value("password").toString(),emsg);
        goto send_ifo;
    case 302:
        this->_register(map.value("username").toString(),map.value("password").toString(),emsg);
        goto send_ifo;
    case 303:
        this->Logout();
        return;
    }
    send_ifo:
    if (isLogin)
        SendLoginSuccessMessage();
    else
    {
        SendLoginErrorMessage(emsg);
    }
}

bool Login::login(QString username, QString password,QString &emsg)
{
    if (readfromfile().value(username).toString()==QString("%1").arg(QString(QCryptographicHash::hash((password+"Server_Sencond_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex())))
    {
        this->username=username;
        isLogin=true;
        return true;
    }
    else
    {
        emsg="Username or Password Error";
        return false;
    }
}

bool Login::_register(QString username, QString password,QString &emsg)
{
    QVariantMap bmap = readfromfile();
    if (bmap.contains(username))
    {
        emsg = "User Exist";
        return false;
    }
    else
    {
        bmap.insert(username,QString("%1").arg(QString(QCryptographicHash::hash((password+"Server_Sencond_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex())));
        writetofile(bmap);
        isLogin=true;
        this->username=username;
        return true;
    }
}

void Login::writetofile(QVariantMap vm)
{
    QFile F(SAVE_AS);
    F.open(QIODevice::ReadWrite);
    F.resize(0);
    F.write(Jsoncoder::encode(vm).toLatin1());
    F.flush();
    F.close();
}

QVariantMap Login::readfromfile()
{
    QFile F(SAVE_AS);
    F.open(QIODevice::ReadWrite);
    return Jsoncoder::deocde(F.readAll());
}
void Login::SendLoginErrorMessage(QString emsg)
{
    ntwkmgr->send(QVariantMap({
                         std::make_pair("status",301),
                         std::make_pair("emsg",emsg)
                     }),LOGIN_HEAD);
}

void Login::SendLoginSuccessMessage()
{
    ntwkmgr->send(QVariantMap({
                         std::make_pair("status",302),
                         std::make_pair("username",username),
                     }),LOGIN_HEAD);
}

void Login::Logout()
{
    isLogin=false;
    username.clear();
    ntwkmgr->send(QVariantMap({
                         std::make_pair("status",303),
                              }),LOGIN_HEAD);
}

void Login::dscnktd()
{
    this->Logout();
    this->deleteLater();
}

//bool Login::list_find(QString username)
//{
//    for (std::list<std::string>::iterator it = userlogged.begin();it != userlogged.end();++it) {
//        if (*it==username.toStdString())
//        {
//            return true;
//        }
//    }
//    return false;
//}

//std::list<std::string> Login::userlogged = std::list<std::string>();
