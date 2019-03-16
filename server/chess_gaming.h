//#ifndef CHESS_GAMING_H
//#define CHESS_GAMING_H

//#include <QObject>
//#include <QVector>
//#include "mainnetworkmanger.h"
//#include <QTimer>
//#include "event.h"

///*
// * Request Respone
// * recv:
// *      101: client ready {+players(QStringList)}
// *      102: drop chess {+x(int) +y(int)}
// * send:
// *      200: server start {}
// *      201: server ready {+while(QString) +black(QString) +usn(QString) +nowhosturn(QString)}
// *      202: chess changed {+x(int) +y(int) +who(QString) +nowwhosturn(QString)}
// *      203: client disconnected {}
// *      204: force game over {}
// *      205: game winner appeared {+winner(QString)}
//*/

//class chess_gaming : public RQSTPRCS
//{
//    Q_OBJECT
//public:
//    explicit chess_gaming(RequestProcesser*parent,int x,int y);
//    ~chess_gaming();
//public slots:
//    void sendReadyed();
//    void sendChessChanged(int x,int y);
//    void sendGameEnd();
//    void sendGameWinner();
//    void recv(QVariantMap);
//    void dscnktd();
//    void sendDisconnected();
//signals:
//    void chesschanged(int x,int y);
//private:
//    void createGame(QStringList);
//    void dropchess(int x,int y);
//    void checkwinner();
//    void init_place();
//    void forceRestGame();
//    int xlen,ylen;
//    QString game_id;
//    struct a_game
//    {
//        QString first;
//        QString second;
//        QString now_whos_turn;
//        QString winner;
//        struct zi{
//            bool isCovered;
//            QString who;
//        };
//        std::vector<std::vector<zi>> pan;

//        chess_gaming* first_add;
//        chess_gaming* second_add;
//    };
//    static QMap<QString,a_game> games;
//    bool isgaming=false;
//};

//#endif // CHESS_GAMING_H
