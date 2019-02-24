#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QFile>
#include "mainnetworkmanger.h"
#define SAVE_AS "user_data.json"
struct LOGIN_DATA{
    bool isSuccess;
    QString reasons;
};

class Login
{
public:
   static bool login(QString username,QString password);
   static bool _register(QString username,QString password);
private:
   static void writetofile(QVariantMap vm);
   static QVariantMap readfromfile();
};

#endif // LOGIN_H
