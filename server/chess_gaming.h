#ifndef CHESS_GAMING_H
#define CHESS_GAMING_H

#include <QObject>
#include <QVector>
#include "requestprocesser.h"
#include <QTimer>
struct a_game
{
    QString first;
    QString second;
    bool isgaming;
    QString winner;
    struct zi{
        bool isCovered;
        QString who;
    };
    QVector<QVector<zi>> pan;
};

extern QMap<QString,a_game> games;

class chess_gaming : public QObject
{
    Q_OBJECT
public:
    explicit chess_gaming(QStringList playerlist,QString ifo,QObject *parent = nullptr,RequestProcesser* mnm = nullptr);

public slots:
    void recv_process(QVariantMap);
private:
    void createGame(QStringList);
    void sendReadyed();
    void dropchess(int x,int y);
    void checkReady();
    void sendChessChanged(int x,int y);
    RequestProcesser* ntwkmgr;
    int pan_w,pan_h;
    QString game_id;
    QString username;
    QTimer tm_fr_chk_rdr;
};

#endif // CHESS_GAMING_H
