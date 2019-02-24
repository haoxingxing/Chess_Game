#ifndef MAINNETWORKMANGER_H
#define MAINNETWORKMANGER_H

#include "loading.h"

#include <QWidget>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#define FM std::make_pair("for","main"),
#define For(x) std::make_pair("for",x),
namespace Ui {
class MainNetworkManger;
}

class MainNetworkManger : public QWidget
{
    Q_OBJECT

public:
    explicit MainNetworkManger(QWidget *parent = nullptr);
    ~MainNetworkManger();
    void send(QString);
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
