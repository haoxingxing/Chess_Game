#ifndef REQUESTPROCESSER_H
#define REQUESTPROCESSER_H

#include <QObject>
#include <QTcpSocket>

#define NETWORK_HEAD "network"
#define MENU_HEAD "menu"
#define CHESS_HEAD "chess_place"
#define LOGIN_HEAD "login"
#define RANK_HEAD "rank"
#define CHAT_HEAD "chat"
#define EID_LEN 8
#define SID_LEN 6

#define JSON_ACT "act"
#define JSON_ARG "arg"
#define JSON_ERROR_ID "eid"
#define JSON_ERROR_STR "err"
#define JSON_EVENT_ID "evid"
#define JSON_MODE "mode"
#define JSON_NEW_EVENT_ID "id"

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
