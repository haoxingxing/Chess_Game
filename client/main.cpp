#include "mainnetworkmanger.h"
#include <QApplication>
#include <QDebug>
#include "jsoncoder.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainNetworkManger w;
    w.show();

    return a.exec();
}
