#ifndef MAINNETWORKMANGER_H
#define MAINNETWORKMANGER_H

#include "loading.h"

#include <QWidget>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#define MAIN_HEAD "main"
#define CHESS_HEAD "chess_place"
#define LOGIN_HEAD "login"
#define RANK_HEAD "rank"
#define CHAT_HEAD "chat"

#define JSON_ACT "act"
#define JSON_ARG "arg"
#define JSON_ERROR_ID "eid"
#define JSON_ERROR_STR "err"
#define JSON_EVENT_ID "evid"
#define JSON_MODE "mode"
#define JSON_NEW_EVENT_ID "id"

namespace Ui {
class MainNetworkManger;
}
class Event;
class MainNetworkManger : public QWidget
{
    Q_OBJECT

public:
    explicit MainNetworkManger(QWidget *parent = nullptr);
    ~MainNetworkManger();
    void sendraw(const QVariantMap &args);
    void sendevt(const int &sid,const QString &evid,const QVariantMap &args);
    void sendnev(const int &id);
    QString username;
signals:
    void Message(QVariantMap);
    void Disconnect();
public slots:
    void Connected();
    void ReadyRead();
    void Timeout();
    void Disconnected();
private slots:
    void on_connect_clicked();

private:
    Event* NewEvent(int id,QString evid);

    QTcpSocket *socket = new QTcpSocket;
    Ui::MainNetworkManger *ui;
    QTimer tmr_for_timeout;
    QVector<QWidget*> wds;
    QMap<QString,Event*> events;
};

#endif // MAINNETWORKMANGER_H
