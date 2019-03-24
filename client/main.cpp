#include "mainnetworkmanger.h"
#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QTextCodec>
#include "jsoncoder.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    a.setWindowIcon(QIcon(":/main.ico"));
    MainNetworkManger w;
    w.show();

    return a.exec();
}
#error PENDING: PROGRAM STOPPED TILL IF IT IS NECESSARY