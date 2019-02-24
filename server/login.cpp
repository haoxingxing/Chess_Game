#include "jsoncoder.h"
#include "login.h"

#include <QCryptographicHash>


bool Login::login(QString username, QString password)
{
    if (readfromfile().value(username).toString()==QString("%1").arg(QString(QCryptographicHash::hash((password+"Server_Sencond_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex())))
        return true;
    else
        return false;
}

bool Login::_register(QString username, QString password)
{
    QVariantMap bmap = readfromfile();
    if (bmap.contains(username))
        return false;
    else
    {
        bmap.insert(username,QString("%1").arg(QString(QCryptographicHash::hash((password+"Server_Sencond_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex())));
        writetofile(bmap);
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
