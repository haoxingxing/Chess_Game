#include "jsoncoder.h"
#include "ranking.h"
#include <QDebug>
#include "login.h"
#include <qthread.h>
#include "chess_gaming.h"
ranking::ranking(RequestProcesser* mnm,int n,QString rank_info) : RQSTPRCS (RANK_HEAD,mnm),nb(n)
{
    rkif=rank_info;
    this->SendRankStartInfo();
    rkn=new rank_node(this,nb,rank_info);
    connect(rkn,&rank_node::fulled,this,&ranking::SendFulled);
    connect(rkn,&rank_node::number_changed,this,&ranking::SendValueChanged);
    connect(ntwkmgr,&RequestProcesser::dscnktd,this,[&]{
        rkn->del(ntwkmgr->verify->username);
    });
}

void ranking::SendRankStartInfo()
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",102)
                              }),MENU_HEAD);
}

void ranking::SendJoinRankFailedInfo()
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",100)
                              }),RANK_HEAD);
}

void ranking::SendValueChanged(int now,QStringList list)
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",103),
                                  std::make_pair("nnb",now),
                                  std::make_pair("list",list)
                              }),RANK_HEAD);
}
void ranking::SendFulled(QStringList strlist)
{

    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",101),
                                  std::make_pair("players",strlist),
                              }),RANK_HEAD);
    this->deleteLater();
}
void ranking::recv(QVariantMap map)
{
    switch (map.value("status").toInt()) {
     case 101:
        rkn->del(ntwkmgr->verify->username);
        ntwkmgr->send(QVariantMap({
                                      std::make_pair("status",105),
                                  }),RANK_HEAD);
        ntwkmgr->send(QVariantMap({
                                      std::make_pair("status",103),
                                  }),MENU_HEAD);
        this->deleteLater();
        break;
    case 102:
        if (!this->Join())
            SendJoinRankFailedInfo();
        else
            SendJoined();
        break;
    }
}

void ranking::dscnktd()
{
    rkn->del(ntwkmgr->verify->username);
    this->deleteLater();
}

bool ranking::Join()
{
    return rkn->join(ntwkmgr->verify->username);
}

void ranking::SendJoined()
{
    ntwkmgr->send(QVariantMap({
                                  std::make_pair("status",104),
                              }),RANK_HEAD);
}



rank_node::rank_node(QObject *parent, int n,QString rank_info) : QObject (parent),nb(n),rkif(rank_info){
    connect(&tm_ch_sg,&QTimer::timeout,this,&rank_node::check_full);
    connect(&tm_ch_sg,&QTimer::timeout,this,&rank_node::check_number_changed);
}

bool rank_node::join(QString username)
{
    if (!isranking)
    {
        if (rn[QString::number(nb)+"*"+rkif].now!=nb)
        {
            check_full();
            rn[QString::number(nb)+"*"+rkif].now++;
            rn[QString::number(nb)+"*"+rkif].user_list.append(username);
            now_in_this_class=0;
            isranking=true;
            tm_ch_sg.start(1000);
            check_full();
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void rank_node::del(QString username)
{
    if (isranking)
        for(int i=0;i<rn[QString::number(nb)+"*"+rkif].user_list.length();++i)
            if (rn[QString::number(nb)+"*"+rkif].user_list[i]==username)
            {
                rn[QString::number(nb)+"*"+rkif].user_list.removeAt(i);
                --rn[QString::number(nb)+"*"+rkif].now;
                tm_ch_sg.stop();
                isranking=false;
                break;
            }
}

void rank_node::check_number_changed()
{
    if (isranking)
        if (rn.contains(QString::number(nb)+"*"+rkif)) {
            if (rn[QString::number(nb)+"*"+rkif].now!=now_in_this_class)
            {
                now_in_this_class=rn[QString::number(nb)+"*"+rkif].now;
                emit number_changed(now_in_this_class,rn[QString::number(nb)+"*"+rkif].user_list);
            }
        }
}

void rank_node::check_full()
{
    if (isranking){
        if (rn.contains(QString::number(nb)+"*"+rkif)) {
            if (rn[QString::number(nb)+"*"+rkif].now==nb)
            {
                isranking=false;
                tm_ch_sg.stop();
                if (rn[QString::number(nb)+"*"+rkif].fulledSiganlSended==nb - 1){
                    emit fulled(rn[QString::number(nb)+"*"+rkif].user_list);
                    rn.remove(QString::number(nb)+"*"+rkif);
                }
                else {
                    emit fulled(rn[QString::number(nb)+"*"+rkif].user_list);
                    rn[QString::number(nb)+"*"+rkif].fulledSiganlSended++;
                }
            }
        }
    }
}

QMap<QString,rank_node::Rank_Node> rank_node::rn = QMap<QString,rank_node::Rank_Node>();
