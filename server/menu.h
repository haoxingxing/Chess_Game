#ifndef MENU_H
#define MENU_H

#include <QObject>
#include "event.h"
#include "eventmanger.h"
class menu : public Event
{
    Q_OBJECT
public:
    menu(MainNetworkManger*rp,QString evid,EventManger* e);
    void recv(const int&s, const QVariantMap&);
    void rank_finished(QStringList l);
private:
    QString rank_event_id;
};

#endif // MENU_H
