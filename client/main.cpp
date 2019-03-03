#include "mainnetworkmanger.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include "jsoncoder.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    MainNetworkManger w;
    w.show();

    return a.exec();
}
