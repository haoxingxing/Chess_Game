#include "chess_gaming.h"
#include "jsoncoder.h"
#include "login.h"
QMap<QString,chess_gaming::a_game> chess_gaming::games = QMap<QString,chess_gaming::a_game>();

chess_gaming::chess_gaming(RequestProcesser*parent,int x,int y) :
    RQSTPRCS(CHESS_HEAD,parent),
    xlen(x),
    ylen(y)
{
    connect(this,&chess_gaming::chesschanged,this,&chess_gaming::sendChessChanged);
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",200)
                              }),CHESS_HEAD);
}

chess_gaming::~chess_gaming()
{
    games.remove(game_id);
}


void chess_gaming::recv(QVariantMap map)
{
    switch(map.value("status").toInt()){
    case 101:
    {
        createGame(map.value("players").toStringList());
        break;
    }
    case 102:
        dropchess(map.value("x").toInt(),map.value("y").toInt());
        break;
    }

}
void chess_gaming::sendChessChanged(int x, int y)
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",202),
                                  std::make_pair("x",x),
                                  std::make_pair("y",y),
                                  std::make_pair("who",games[game_id].pan[size_t(x)+5][size_t(y)+5].who),
                                  std::make_pair("nowhosturn",games[game_id].now_whos_turn)
                              }),CHESS_HEAD);
    this->checkwinner();
}

void chess_gaming::sendGameEnd()
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",204),
                              }),CHESS_HEAD);
    this->deleteLater();
}

void chess_gaming::sendGameWinner()
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",205),
                                  std::make_pair("winner",games[game_id].winner)
                              }),CHESS_HEAD);
    this->deleteLater();
}

void chess_gaming::sendReadyed()
{
    isgaming=true;
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",201),
                                  std::make_pair("white",games[game_id].first),
                                  std::make_pair("black",games[game_id].second),
                                  std::make_pair("usn",ntwkmgr->verify->username),
                                  std::make_pair("nowhosturn",games[game_id].now_whos_turn)
                              }),CHESS_HEAD);
}


void chess_gaming::dscnktd()
{
    if (isgaming){
        this->forceRestGame();
    }

}

void chess_gaming::sendDisconnected()
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",203),
                              }),CHESS_HEAD);
}

void chess_gaming::createGame(QStringList gamers)
{
    game_id=gamers.join("*");
    if (!games.contains(game_id))
    {
        games.insert(game_id,a_game());
        games[game_id].first=gamers[0];
        games[game_id].second=gamers[1];
        if (gamers[0]==ntwkmgr->verify->username)
        {
            games[game_id].first_add=this;
        }
        else {
            games[game_id].second_add=this;
        }
    }
    else {
        if (gamers[0]==ntwkmgr->verify->username)
        {
            games[game_id].first_add=this;
        }
        else {
            games[game_id].second_add=this;
        }
        games[game_id].now_whos_turn=games[game_id].first;
        init_place();
        connect(games[game_id].first_add,&chess_gaming::chesschanged,games[game_id].second_add,&chess_gaming::sendChessChanged);
        connect(games[game_id].second_add,&chess_gaming::chesschanged,games[game_id].first_add,&chess_gaming::sendChessChanged);
        games[game_id].first_add->sendReadyed();
        games[game_id].second_add->sendReadyed();
    }
}


void chess_gaming::dropchess(int x, int y)
{
    if (games[game_id].now_whos_turn==ntwkmgr->verify->username)
        if (games[game_id].pan[size_t(x)+5][size_t(y)+5].who==""){
            games[game_id].pan[size_t(x)+5][size_t(y)+5].who=ntwkmgr->verify->username;
            games[game_id].now_whos_turn=(games[game_id].now_whos_turn==games[game_id].first)?games[game_id].second:games[game_id].first;
            emit chesschanged(x,y);
        }
}

void chess_gaming::checkwinner()
{
    if (isgaming)
    {
        for(size_t x=5;x<games[game_id].pan.size()-5;++x)
        {
            for (size_t y=5;y<games[game_id].pan[x].size()-5;++y) {
                if (games[game_id].pan[x][y].who==games[game_id].first&&games[game_id].pan[x+1][y].who==games[game_id].first&&games[game_id].pan[x+2][y].who==games[game_id].first&&games[game_id].pan[x+3][y].who==games[game_id].first&&games[game_id].pan[x+4][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                else if (games[game_id].pan[x][y].who==games[game_id].first&&games[game_id].pan[x-1][y].who==games[game_id].first&&games[game_id].pan[x-2][y].who==games[game_id].first&&games[game_id].pan[x-3][y].who==games[game_id].first&&games[game_id].pan[x-4][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                else if (games[game_id].pan[x][y+1].who==games[game_id].first&&games[game_id].pan[x][y+2].who==games[game_id].first&&games[game_id].pan[x][y+3].who==games[game_id].first&&games[game_id].pan[x][y+4].who==games[game_id].first&&games[game_id].pan[x][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                else if (games[game_id].pan[x][y-1].who==games[game_id].first&&games[game_id].pan[x][y-2].who==games[game_id].first&&games[game_id].pan[x][y-3].who==games[game_id].first&&games[game_id].pan[x][y-4].who==games[game_id].first&&games[game_id].pan[x][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                else if (games[game_id].pan[x+1][y+1].who==games[game_id].first&&games[game_id].pan[x+2][y+2].who==games[game_id].first&&games[game_id].pan[x+3][y+3].who==games[game_id].first&&games[game_id].pan[x+4][y+4].who==games[game_id].first&&games[game_id].pan[x][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                else if (games[game_id].pan[x-1][y-1].who==games[game_id].first&&games[game_id].pan[x-2][y-2].who==games[game_id].first&&games[game_id].pan[x-3][y-3].who==games[game_id].first&&games[game_id].pan[x-4][y-4].who==games[game_id].first&&games[game_id].pan[x][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                else if (games[game_id].pan[x-1][y+1].who==games[game_id].first&&games[game_id].pan[x-2][y+2].who==games[game_id].first&&games[game_id].pan[x-3][y+3].who==games[game_id].first&&games[game_id].pan[x-4][y+4].who==games[game_id].first&&games[game_id].pan[x][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                else if (games[game_id].pan[x+1][y-1].who==games[game_id].first&&games[game_id].pan[x+2][y-2].who==games[game_id].first&&games[game_id].pan[x+3][y-3].who==games[game_id].first&&games[game_id].pan[x+4][y-4].who==games[game_id].first&&games[game_id].pan[x][y].who==games[game_id].first){
                    games[game_id].winner=games[game_id].first;
                    goto has_winner;
                }
                //#
                if (games[game_id].pan[x][y].who==games[game_id].second&&games[game_id].pan[x+1][y].who==games[game_id].second&&games[game_id].pan[x+2][y].who==games[game_id].second&&games[game_id].pan[x+3][y].who==games[game_id].second&&games[game_id].pan[x+4][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
                else if (games[game_id].pan[x][y].who==games[game_id].second&&games[game_id].pan[x-1][y].who==games[game_id].second&&games[game_id].pan[x-2][y].who==games[game_id].second&&games[game_id].pan[x-3][y].who==games[game_id].second&&games[game_id].pan[x-4][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
                else if (games[game_id].pan[x][y+1].who==games[game_id].second&&games[game_id].pan[x][y+2].who==games[game_id].second&&games[game_id].pan[x][y+3].who==games[game_id].second&&games[game_id].pan[x][y+4].who==games[game_id].second&&games[game_id].pan[x][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
                else if (games[game_id].pan[x][y-1].who==games[game_id].second&&games[game_id].pan[x][y-2].who==games[game_id].second&&games[game_id].pan[x][y-3].who==games[game_id].second&&games[game_id].pan[x][y-4].who==games[game_id].second&&games[game_id].pan[x][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
                else if (games[game_id].pan[x+1][y+1].who==games[game_id].second&&games[game_id].pan[x+2][y+2].who==games[game_id].second&&games[game_id].pan[x+3][y+3].who==games[game_id].second&&games[game_id].pan[x+4][y+4].who==games[game_id].second&&games[game_id].pan[x][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
                else if (games[game_id].pan[x-1][y-1].who==games[game_id].second&&games[game_id].pan[x-2][y-2].who==games[game_id].second&&games[game_id].pan[x-3][y-3].who==games[game_id].second&&games[game_id].pan[x-4][y-4].who==games[game_id].second&&games[game_id].pan[x][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
                else if (games[game_id].pan[x-1][y+1].who==games[game_id].second&&games[game_id].pan[x-2][y+2].who==games[game_id].second&&games[game_id].pan[x-3][y+3].who==games[game_id].second&&games[game_id].pan[x-4][y+4].who==games[game_id].second&&games[game_id].pan[x][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
                else if (games[game_id].pan[x+1][y-1].who==games[game_id].second&&games[game_id].pan[x+2][y-2].who==games[game_id].second&&games[game_id].pan[x+3][y-3].who==games[game_id].second&&games[game_id].pan[x+4][y-4].who==games[game_id].second&&games[game_id].pan[x][y].who==games[game_id].second){
                    games[game_id].winner=games[game_id].second;
                    goto has_winner;
                }
            }
        }
        return;
has_winner:{
            this->sendGameWinner();
        }
    }
}

void chess_gaming::init_place()
{
    for (int x=0;x<=xlen+15;++x)
    {
        games[game_id].pan.push_back(std::vector<a_game::zi>());
        for (int y=0;y<=ylen+15;++y)
        {
            games[game_id].pan[size_t(x)].push_back(a_game::zi());
        }
    }
}

void chess_gaming::forceRestGame()
{
    if (isgaming){
        games[game_id].first_add->sendGameEnd();
        games[game_id].second_add->sendGameEnd();
        games.remove(game_id);
    }
}

