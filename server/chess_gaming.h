#ifndef CHESS_GAMING_H
#define CHESS_GAMING_H

#include <QObject>
#include <QVector>
#include "requestprocesser.h"
#include <QTimer>
#include "rqstprcs.h"

class chess_gaming : public RQSTPRCS
{
    Q_OBJECT
public:
    explicit chess_gaming(RequestProcesser*parent,int x,int y);
    ~chess_gaming();
public slots:
    void sendReadyed();
    void sendChessChanged(int x,int y);
    void sendGameEnd();
    void sendGameWinner();
    void recv(QVariantMap);
    void dscnktd();
    void sendDisconnected();
signals:
    void chesschanged(int x,int y);
private:
    void createGame(QStringList);
    void dropchess(int x,int y);
    void checkwinner();
    void init_place();
    void forceRestGame();
    int xlen,ylen;
    QString game_id;    
    struct a_game
    {
        QString first;
        QString second;
        QString now_whos_turn;
        QString winner;
        struct zi{
            bool isCovered;
            QString who;
        };
        std::vector<std::vector<zi>> pan;

        chess_gaming* first_add;
        chess_gaming* second_add;
    };
    static QMap<QString,a_game> games;
    bool isgaming=false;
};

#endif // CHESS_GAMING_H
