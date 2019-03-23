#ifndef MENU_H
#define MENU_H

#include <QObject>
#include "event.h"
#include "eventmanger.h"
class Menu : public Event
{
    Q_OBJECT
public:
    Menu(MainNetworkManger*rp,QString evid,EventManger* e);
    void recv(const int&s, const QVariantMap&);
private:
    QString rank_event_id;
};

#endif // MENU_H
