#include "jsoncoder.h"
#include "ranking.h"
#include <QDebug>
#include <qthread.h>
#include "chess_gaming.h"
ranking::ranking(QObject *parent,RequestProcesser* mnm,int n,QString _username,QString rank_info,QString to) : QObject(parent),nb(n),username(_username),ntwkmgr(mnm)
{    
    rkif=rank_info;
    this->SendRankStartInfo();
    this->to=to;
    rkn=new rank_node(this,nb,rank_info);
    connect(rkn,&rank_node::fulled,this,&ranking::SendFulled);
    connect(rkn,&rank_node::number_changed,this,&ranking::SendValueChanged);
    connect(ntwkmgr,&RequestProcesser::Message,this,&ranking::recvs);
    connect(ntwkmgr,&RequestProcesser::dscnktd,this,[&]{
        rkn->del(username);
    });
    if (!this->Join())
        this->SendJoinRankFailedInfo();
}

void ranking::SendRankStartInfo()
{
    ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                                    std::make_pair("to","rank"),
                                                    std::make_pair("numbr",nb),
                                                    std::make_pair("close","menu"),
                                                })));
}

void ranking::SendJoinRankFailedInfo()
{
    ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                                    std::make_pair("status",100)
                                                })));
}

void ranking::SendValueChanged(int now,QStringList list)
{
    ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                                    std::make_pair("nnb",now),
                                                    std::make_pair("list",list)
                                                })));
}

bool ranking::Join()
{
    return rkn->join(username);
}


void ranking::SendFulled(QStringList strlist)
{

    ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                                    std::make_pair("close","rank"),
                                                    std::make_pair("to",to),
                                                    std::make_pair("players",strlist),
                                                    std::make_pair("you",username),
                                                    std::make_pair("rank_info",rkif)
                                                })));
    new chess_gaming(strlist,rkif,ntwkmgr,ntwkmgr);
    this->deleteLater();
}

void ranking::recvs(QVariantMap map)
{
    if (map.value("for").toString()=="rank")
    {
        if (map.value("action")=="exit")
        {
            rkn->del(username);
            ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                                   std::make_pair("to","menu"),
                                                   std::make_pair("username",username),
                                                   std::make_pair("close","rank")
                                               })));
            this->deleteLater();
        }
    }
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
