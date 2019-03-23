#include "mainnetworkmanger.h"
#include "ui_mainnetworkmanger.h"
#include "jsoncoder.h"
#include "login.h"
#include "menu.h"
#include "ranking.h"
#include "chess_place.h"
#include "event.h"
#include <qmessagebox.h>
MainNetworkManger::MainNetworkManger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainNetworkManger)
{
    ui->setupUi(this);
    ui->connect->hide();
    this->show();
    ui->status->setText("Please Check the address");
    ui->connect->show();
    connect(socket,&QTcpSocket::connected,this,&MainNetworkManger::Connected);
    connect(socket,&QTcpSocket::readyRead,this,&MainNetworkManger::ReadyRead);
    connect(socket,&QTcpSocket::disconnected,this,&MainNetworkManger::Disconnected);
    connect(&tmr_for_timeout,&QTimer::timeout,this,&MainNetworkManger::Timeout);
}

MainNetworkManger::~MainNetworkManger()
{
    delete ui;
}

void MainNetworkManger::sendraw(const QVariantMap &args)
{
    qDebug() << "\033[33m" <<Jsoncoder::encode(args).toUtf8().toStdString().c_str() << "\033[0m";
    socket->write(Jsoncoder::encode(args).toUtf8() + "\r\n");
    socket->flush();
}

void MainNetworkManger::sendevt(const int &sid,const QString &evid, const QVariantMap &args)
{
    QVariantMap map;
    map.insert(JSON_ACT,sid);
    map.insert(JSON_EVENT_ID,evid);
    map.insert(JSON_ARG,args);
    this->sendraw(map);
}
void MainNetworkManger::sendnev(const int &id)
{
    QVariantMap map;
    map.insert(JSON_EVENT_ID,"new");
    map.insert(JSON_NEW_EVENT_ID,id);
    this->sendraw(map);
}

void MainNetworkManger::Connected()
{
    tmr_for_timeout.stop();
    ui->status->setText("connected");
    this->hide();
}

void MainNetworkManger::ReadyRead()
{
    QString data = socket->readLine();
    qDebug() << data.toUtf8().toStdString().c_str();
    do{
        QVariantMap map=Jsoncoder::deocde(data);
        switch (map.value(JSON_MODE).toInt())
        {
        case 1:
            events.insert(map.value(JSON_EVENT_ID).toString(),NewEvent(map.value(JSON_NEW_EVENT_ID).toInt(),map.value(JSON_EVENT_ID).toString()));
            break;
        case 2:
            if (map.value(JSON_EVENT_ID).toString() != "new")
            {
                events[map.value(JSON_EVENT_ID).toString()]->err(map.value(JSON_ERROR_ID).toInt(),map.value(JSON_ERROR_STR).toString());
            }
            else
            {
                QMessageBox::critical(nullptr,QString::number(map[JSON_ERROR_ID].toInt()),map[JSON_ERROR_STR].toString());
            }
            break;
        case 3:
            if (events.contains(map.value(JSON_EVENT_ID).toString()))
                events[map.value(JSON_EVENT_ID).toString()]->recv_t(map.value(JSON_ACT).toInt(),map.value(JSON_ARG).toMap());
            break;
        }
        data.clear();
    }
    while ((data=socket->readLine())!="");
}

void MainNetworkManger::Timeout()
{
    tmr_for_timeout.stop();
    socket->disconnectFromHost();
    ui->status->setText("failed");
    ui->address->show();
    ui->port->show();
    ui->connect->show();
    ui->label_2->show();
    this->activateWindow();
}

void MainNetworkManger::Disconnected()
{
    emit Disconnect();
    this->show();
    ui->status->setText("disconnected");
    ui->address->show();
    ui->port->show();
    ui->connect->show();
    ui->label_2->show();
    foreach (Event*p,events)
    {
        delete p;
    }
    events.clear();
    this->activateWindow();
}

void MainNetworkManger::on_connect_clicked()
{
    ui->connect->hide();
    ui->address->hide();
    ui->port->hide();
    ui->label_2->hide();
    socket->connectToHost(ui->address->text(),quint16(ui->port->text().toInt()));
    tmr_for_timeout.start(10000);
    ui->status->setText("connecting");

}

Event* MainNetworkManger::NewEvent(int id,QString evid)
{
    switch(id)
    {
    case 1:
        return new login(this,nullptr,evid);
    case 2:
        return new menu(this,nullptr,evid);
    }
    return nullptr;
}
