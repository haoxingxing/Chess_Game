#include "chess_gaming.h"
#include "jsoncoder.h"

QMap<QString,a_game> games = QMap<QString,a_game>();

chess_gaming::chess_gaming(QStringList plrlst,QString ifo,QObject *parent,RequestProcesser* mnm) :
    QObject(parent),
    ntwkmgr(mnm)
{
    pan_w=ifo.split("*")[0].toInt();
    pan_h=ifo.split("*")[1].toInt();
    connect(ntwkmgr,&RequestProcesser::Message,this,&chess_gaming::recv_process);
    connect(ntwkmgr,&RequestProcesser::dscnktd,this,[&]{

    });
    username=ntwkmgr->username;
    connect(&tm_fr_chk_rdr,&QTimer::timeout,this,&chess_gaming::checkReady);
    tm_fr_chk_rdr.start(1000);
    createGame(plrlst);
}

void chess_gaming::recv_process(QVariantMap map)
{
    if (map.value("for").toString()=="chess_place")
    {
        if (map.value("action")=="get_all")
        {
            QVariantMap map;
            for (int x=0;x<=games[game_id].pan.length();++x)
                for (int y=0;y<=games[game_id].pan[x].length();++y)
                {
                    map.insert(QString::number(x)+"*"+QString::number(y),games[game_id].pan[x][y].who);
                }
            map.insert("status",203);
            ntwkmgr->send(Jsoncoder::encode(map));
        }
        if (map.value("action")=="drop")
        {
            dropchess(map.value("x").toInt(),map.value("y").toInt());
        }
    }
}

void chess_gaming::createGame(QStringList gamers)
{
    game_id=gamers.join("*");
    if (!games.contains(game_id))
    {
        games.insert(game_id,a_game());
        games[game_id].first=gamers[0];
        games[game_id].second=gamers[1];
        games[game_id].isgaming=false;
    }
    else {
        games[game_id].isgaming=true;
        sendReadyed();
    }
}

void chess_gaming::sendReadyed()
{
    tm_fr_chk_rdr.stop();
    ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                           std::make_pair("status",201),
                                                })));
}

void chess_gaming::dropchess(int x, int y)
{
    if (games[game_id].pan[x][y].who==""){
        games[game_id].pan[x][y].who=username;
        this->sendChessChanged(x,y);
    }
}

void chess_gaming::checkReady()
{
    if (games[game_id].isgaming)
        sendReadyed();
}

void chess_gaming::sendChessChanged(int x, int y)
{
    ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                           std::make_pair("status",202),
                                           std::make_pair("x",x),
                                           std::make_pair("y",y),
                                           std::make_pair("who",games[game_id].pan[x][y].who)
                                                })));
}
