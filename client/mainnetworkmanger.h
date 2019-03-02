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
namespace Ui {
class MainNetworkManger;
}

class MainNetworkManger : public QWidget
{
    Q_OBJECT

public:
    explicit MainNetworkManger(QWidget *parent = nullptr);
    ~MainNetworkManger();
    void send(QVariantMap,QString);
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
    QTcpSocket *socket = new QTcpSocket;
    Ui::MainNetworkManger *ui;
    QTimer tmr_for_timeout;
    QVector<QWidget*> wds;
};

#endif // MAINNETWORKMANGER_H
