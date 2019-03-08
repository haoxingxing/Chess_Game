#include "mainnetworkmanger.h"
#include "ui_mainnetworkmanger.h"
#include "jsoncoder.h"
#include "login.h"
#include "menu.h"
#include "ranking.h"
#include "chess_place.h"
MainNetworkManger::MainNetworkManger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainNetworkManger)
{
    ui->setupUi(this);
    ui->connect->hide();

    connect(socket,&QTcpSocket::connected,this,&MainNetworkManger::Connected);
    connect(socket,&QTcpSocket::readyRead,this,&MainNetworkManger::ReadyRead);
    connect(socket,&QTcpSocket::disconnected,this,&MainNetworkManger::Disconnected);
    connect(&tmr_for_timeout,&QTimer::timeout,this,&MainNetworkManger::Timeout);
    socket->connectToHost(ui->address->text(),quint16(ui->port->text().toInt()));
    ui->status->setText("connecting");
    ui->connect->hide();
    ui->address->hide();
    ui->port->hide();
    ui->label_2->hide();
    tmr_for_timeout.start(10000);
}

MainNetworkManger::~MainNetworkManger()
{
    delete ui;
}

void MainNetworkManger::send(QVariantMap map,QString head)
{
    map.insert("for",head);
    socket->write((Jsoncoder::encode(map) +"\r\n").toUtf8());
    socket->flush();
}

void MainNetworkManger::Connected()
{
    tmr_for_timeout.stop();
    ui->status->setText("connected");
    (new login(this))->show();
    this->hide();
}

void MainNetworkManger::ReadyRead()
{
    QString data = socket->readLine();
    qDebug() << data;
    do{
        QVariantMap map=Jsoncoder::deocde(data);
        if (map.value("for").toString().contains("network"))
        {
            if (map.value("hide").toString().contains("network"))
                this->hide();
            if (map.value("show").toString().contains("network"))
                this->show();
            if (map.value("close").toString().contains("network"))
                exit(EXIT_SUCCESS);
        }
        emit Message(map);
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
