#include <QCoreApplication>
#include "mainnetworkmanger.h"
#include "login.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainNetworkManger n;

    return a.exec();
}
