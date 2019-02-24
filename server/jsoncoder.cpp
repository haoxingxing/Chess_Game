#include "jsoncoder.h"

#include <QJsonDocument>
#include <qvariant.h>
#include <QDebug>
QVariantMap Jsoncoder::deocde(QString str)
{
    return QJsonDocument::fromJson(str.toLatin1()).toVariant().toMap();
}

QString Jsoncoder::encode(QVariantMap KV)
{
    return  QJsonDocument::fromVariant(QVariant(KV)).toJson(QJsonDocument::Compact);
}
