#include <QCoreApplication>
#include "mainnetworkmanger.h"
#include "login.h"
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    MainNetworkManger n;

    return a.exec();
}

/* class chat
 * recv:
 *      100: join {+channel(QString)}
 *      101: exit {+channel(QString)}
 *      102: send {+message(QString) +tochannel(QString)}
 *
 * send:
 *      200: server ready {}
 *      201: joined {+channel(QString)}
 *      202: exited {+channel(QString)}
 *      203: new message {+channel(QString) +from(QString) +msg(QString)}
 * %%/IGORN     204: new user joined {+channel(QString) +who(QString)}
 * %%/IGORN     205: user exited {+channel(QString) +who(QString)}
 *
 * class chess_gaming
 * recv:
 *      101: client ready {+players(QStringList)}
 *      102: drop chess {+x(int) +y(int)}
 * send:
 *      200: server start {}
 *      201: server ready {+while(QString) +black(QString) +usn(QString) +nowhosturn(QString)}
 *      202: chess changed {+x(int) +y(int) +who(QString) +nowwhosturn(QString)}
 *      203: client disconnected {}
 *      204: force game over {}
 *      205: game winner appeared {+winner(QString)}
 *
 * class login
 * recv:
 *      301: login {+username(QString) +password(QString)}
 *      302: register {+username(QString) +password(QString)}
 *      303: logout {}
 * send:
 *      301: lr error {+emsg(QString)}
 *      302: lr success {+username(QString)}
 *      303: logout successful {}
 *
 * class ranking
 * recv:
 *      101: exit {}
 *      102: join {}
 * send:
 *      100: join failed {}
 *      101: rank fulled {+players(QStringList)}
 *      102: rank started {}
 *      103: rank list changed {+nnb(int) +list(QStringList)}
 *      104: joined {}
*/
