#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QFile>
#include "server.h"
#include "event.h"
#include "eventmanger.h"
#define SAVE_AS "user_data.json"

class Login : public Event
{
public:
   Login(MainNetworkManger*rp,QString evid,EventManger*);
   void recv(const int&,const QVariantMap&);
   bool login(QString username,QString password);
   bool _register(QString username,QString password);
   void Logout();
   QString username;
   bool isLogin = false;
private:
   static void writetofile(QVariantMap vm);
   static QVariantMap readfromfile();
   void changeStatus();
   EventManger* manger;
};

#endif // LOGIN_H
