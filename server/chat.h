//#ifndef CHAT_H
//#define CHAT_H

//#include <QObject>
//#include "mainnetworkmanger.h"
//#include "event.h"

///* Request Respone
// * recv:
// *      100: join {+channel(QString)}
// *      101: exit {+channel(QString)}
// *      102: send {+message(QString) +tochannel(QString)}
// *
// * send:
// *      200: server ready {}
// *      201: joined {+channel(QString)}
// *      202: exited {+channel(QString)}
// *      203: new message {+channel(QString) +from(QString) +msg(QString)}
// *      204: new user joined {+channel(QString) +who(QString)}
// *      205: user exited {+channel(QString) +who(QString)}
// *
//*/
//class chat : public RQSTPRCS
//{
//    Q_OBJECT
//public:
//    explicit chat(MainNetworkManger *parent);
//    void connectMessage(QString);
//    void disconnectMessage(QString);
//    ~chat();
//    void recv(QVariantMap);
//    void dscnktd();
//public slots:
//    void havenewChat(QString cn,QString from,QString);
//    void sendJoined_(QString who,QString from);
//    void sendExited_(QString who,QString from);
//private:
//    void sendReady();
//    void sendJoined(QString cn);
//    void sendExited(QString cn);
//    class chat_channel : public QObject{
//    public:
//        explicit chat_channel(QString name);
//        void join(chat*);
//        void exit(chat*,QString yn);
//        void newMessage(QString from,QString s);
//    private:
//        QVector<chat*> all_members;
//        QString this_n;
//    };
//    QString username;
//    static QMap<QString,chat_channel*> addrs;
//    bool isDeleted = false;
//};

//#endif // CHAT_H
