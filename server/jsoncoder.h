#ifndef JSONCODER_H
#define JSONCODER_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QString>
class Jsoncoder
{
public:
    static QVariantMap deocde(QString str);
    static QString encode(QVariantMap);
public slots:
};

#endif // JSONCODER_H
