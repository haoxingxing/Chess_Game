#include "chat.h"
#include "ui_chat.h"

chat::chat(MainNetworkManger* ntwkmgr,QWidget *parent) :
    WindowProcessSlot (CHAT_HEAD,parent,ntwkmgr),
    ui(new Ui::chat)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    this->setGeometry(parent->x()*2+(parent->width()/2),parent->y()*2,parent->width()/2,parent->height()/2);
    ui->commit->setEnabled(false);
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("request","chat"),
                               }),MAIN_HEAD);
    this->appenedmessage("Waiting For Server");
}

chat::~chat()
{
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("status",101),
                                   std::make_pair("channel",cn),
                               }),CHAT_HEAD);
    delete ui;
}

void chat::appenedmessage(QString msg)
{
    ui->msgshow->append(msg);
}

void chat::join(QString cn)
{
    if (isready)
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("status",100),
                                   std::make_pair("channel",cn),
                               }),CHAT_HEAD);
    else {
        this->cn=cn;
    }
}

void chat::exit(QString cn)
{
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("status",101),
                                   std::make_pair("channel",cn),
                               }),CHAT_HEAD);
}

void chat::recv(QVariantMap map)
{
    switch (map.value("status").toInt()) {
    case 200:
        isready=true;
        if (!cn.isEmpty())
            ntwkmgrr->send(QVariantMap({
                                           std::make_pair("status",100),
                                           std::make_pair("channel",cn),
                                       }),CHAT_HEAD);
        break;
    case 201:
        cn=map.value("channel").toString();
        ui->commit->setEnabled(true);
        break;
    case 202:
        this->appenedmessage("Exited "+map.value("channel").toString());
        cn.clear();
        ui->commit->setEnabled(false);
        break;
    case 203:
        this->appenedmessage(map.value("from").toString() + ": " +map.value("msg").toString());
        this->activateWindow();
        break;
    case 204:
        this->appenedmessage(map.value("who").toString() + " Joined");
        break;
    case 205:
        this->appenedmessage(map.value("who").toString() + " Exited");
        break;
    }
}

void chat::on_commit_clicked()
{
    if (isready)
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("status",102),
                                   std::make_pair("tochannel",cn),
                                   std::make_pair("msg",ui->msg->text())
                               }),CHAT_HEAD);
    ui->msg->clear();
}
