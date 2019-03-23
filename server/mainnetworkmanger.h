#ifndef REQUESTPROCESSER_H
#define REQUESTPROCESSER_H

#include <QObject>
#include <QTcpSocket>
#include "../global.h"

class MainNetworkManger : public QObject
{
    Q_OBJECT
public:
    explicit MainNetworkManger(QObject *parent = nullptr,QTcpSocket* so = nullptr);
    void sendraw(const QVariantMap &args);
    void senderr(const int &eid,const QString &evid,const QString &err);
    void sendevt(const int &sid,const QString &evid,const QVariantMap &args);
    void sendnev(const int &id,const QString &evid);
    QString getScid() const;
    void disconnect();
signals:
    void Message(QVariantMap);
    void dscnktd();
public slots:
    void readyRead();
private:
    QString scid;
    QTcpSocket* socket;
};

#endif // REQUESTPROCESSER_H
