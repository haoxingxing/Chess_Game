#include "jsoncoder.h"
#include "ranking.h"
#include <QDebug>
#include "login.h"
#include <qthread.h>
#include "eventmanger.h"
#include "file_codes.h"
#include "chess_gaming.h"
ranking::ranking(MainNetworkManger*rp,QString evid,EventManger* e) : Event(RANK_HEAD,rp,evid,e)
{
    connect(ntwkmgr,&MainNetworkManger::dscnktd,this,[&]{
        if (rkn!=nullptr)
            rkn->del(evtmgr->username);
    });
}

void ranking::SendValueChanged(int i, QStringList l)
{
    sendevt(5,QVariantMap({
                              std::make_pair("now",i),
                              std::make_pair("list",l)
                          }));
}

void ranking::SendFulled(QStringList l)
{
    sendevt(6,QVariantMap({
                              std::make_pair("list",l)
                          }));
    emit fulled(l);
}

void ranking::recv(const int &s, const QVariantMap &a)
{
    switch (s) {
    case 401:
        if (rkn!=nullptr)
        {
            rkn->del(evtmgr->username);
            delete rkn;
        }
        rkn=new rank_node(this,a.value("count").toInt(),a.value("rank_info").toString());
        rkn->join(evtmgr->username);
        sendevt(4,QVariantMap());
        break;
    case 402:
        if (rkn!=nullptr)
        {
            rkn->del(evtmgr->username);
            delete rkn;
        }
        sendevt(7,QVariantMap());
        break;

    }
}

bool ranking::Join()
{
    return rkn->join(evtmgr->username);
}
void ranking::dscnktd()
{
    rkn->del(evtmgr->username);
    this->deleteLater();
}

























rank_node::rank_node(ranking *parent, int n,QString rank_info) : QObject (parent),nb(n),rkif(rank_info),pt(parent){
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
                pt->SendValueChanged(now_in_this_class,rn[QString::number(nb)+"*"+rkif].user_list);
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
                    pt->SendFulled(rn[QString::number(nb)+"*"+rkif].user_list);
                    rn.remove(QString::number(nb)+"*"+rkif);
                }
                else {
                    pt->SendFulled(rn[QString::number(nb)+"*"+rkif].user_list);
                    rn[QString::number(nb)+"*"+rkif].fulledSiganlSended++;
                }
            }
        }
    }
}

QMap<QString,rank_node::Rank_Node> rank_node::rn = QMap<QString,rank_node::Rank_Node>();
