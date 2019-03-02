#ifndef REQUESTPROCESSER_H
#define REQUESTPROCESSER_H

#include <QObject>
#include <QTcpSocket>
#define NETWORK_HEAD "network"
#define MENU_HEAD "menu"
#define CHESS_HEAD "chess_place"
#define LOGIN_HEAD "login"
#define RANK_HEAD "rank"
class Login;
class RequestProcesser : public QObject
{
    Q_OBJECT
public:
    explicit RequestProcesser(QObject *parent = nullptr,QTcpSocket* so = nullptr);
    void send(QVariantMap str,QString _for);
    Login* verify;

signals:
    void Message(QVariantMap);
    void dscnktd();
public slots:
    void readyRead();
private:
    QTcpSocket* socket;
};

#endif // REQUESTPROCESSER_H
