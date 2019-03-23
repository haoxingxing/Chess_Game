#include <QCoreApplication>
#include "server.h"
#include <QString>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    server n;
    return a.exec();
}
