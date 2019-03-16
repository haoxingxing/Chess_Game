#include <QCoreApplication>
#include <QTcpSocket>
#include <iostream>
#include "client.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    client c;
    return a.exec();
}
