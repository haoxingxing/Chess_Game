#include "file_codes.h"
#include "jsoncoder.h"

#include <QString>
#include <QVariant>
#include <QVariantMap>
QString File_Codes::read(int status)
{
    QFile F(CODES_FILE);
    F.open(QIODevice::ReadWrite);
    return Jsoncoder::deocde(F.readAll()).value(QString::number(status)).toString();
}

QVariantMap File_Codes::readmap(QString status)
{
    QFile F(CODES_FILE);
    F.open(QIODevice::ReadWrite);
    return Jsoncoder::deocde(F.readAll()).value(status).toMap();
}

int File_Codes::readint(QString status)
{
    QFile F(CODES_FILE);
    F.open(QIODevice::ReadWrite);
    return Jsoncoder::deocde(F.readAll()).value(status).toInt();
}
