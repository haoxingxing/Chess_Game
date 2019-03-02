#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QFile>
#include "mainnetworkmanger.h"
#include "rqstprcs.h"
#define SAVE_AS "user_data.json"

class Login : public RQSTPRCS
{
public:
   Login(RequestProcesser *rp);
   void recv(QVariantMap);
   bool login(QString username,QString password,QString &emsg);
   bool _register(QString username,QString password,QString &emsg);
   void Logout();
   void dscnktd();
   QString username;
   bool isLogin = false;
private:
   void SendLoginErrorMessage(QString emsg);
   void SendLoginSuccessMessage();
   static void writetofile(QVariantMap vm);
   static QVariantMap readfromfile();
};

#endif // LOGIN_H
