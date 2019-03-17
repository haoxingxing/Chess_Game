#include "eventwidget.h"

EventWidget::EventWidget(QWidget *parent,MainNetworkManger* ntwkmgr,QString evid) :
    QWidget(parent),Event (ntwkmgr,evid)
{
    send(101,QVariantMap());
}

EventWidget::~EventWidget()
{
}

void EventWidget::recv_t(int status, QVariantMap map)
{
    switch(status)
    {
    case 3:
        this->SetWindowInfo(map);
        break;
    case 10:
        this->close();
        this->deleteLater();
        break;
    default:
        this->recv(status,map);
    }
}

void EventWidget::SetWindowInfo(QVariantMap args)
{

    //this->resize((args.contains("height"))?args["height"].toInt():this->height(),(args.contains("height"))?args["width"].toInt():this->width());
    this->setWindowTitle(args.contains("title")?args["title"].toString():this->windowTitle());
    this->setStyleSheet(args["stylesheet"].toString());
    if (args["isshow"].toBool())
        this->show();
    else {
        this->hide();
    }
}
