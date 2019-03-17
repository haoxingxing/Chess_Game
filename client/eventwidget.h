#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include <QWidget>
#include "event.h"
class EventWidget : public QWidget,public Event
{
    Q_OBJECT

public:
    explicit EventWidget(QWidget *parent,MainNetworkManger* ntwkmgr,QString evid);
    ~EventWidget();
    void recv_t(int status,QVariantMap map);
    virtual void SetWindowInfo(QVariantMap);
private:
};

#endif // EVENTWIDGET_H
