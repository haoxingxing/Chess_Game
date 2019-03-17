#include "menu.h"
#include "eventmanger.h"
#include "file_codes.h"
#include "ranking.h"
menu::menu(MainNetworkManger *rp,QString evid,EventManger* e):Event(MENU_HEAD,rp,evid,e){

}

void menu::recv(const int & s, const QVariantMap &)
{
    switch (s) {
    case 301:
    {
        rank_event_id=evtmgr->NewEvent(3);
        Event* e=evtmgr->GetEvent(rank_event_id);
        e->recv(401,File_Codes::readmap("menu_rank_1"));
        connect(dynamic_cast<ranking*>(e),&ranking::fulled,this,&menu::rank_finished);
        sendevt(8,QVariantMap());
        break;
    }
    case 302:
        foreach(Event* p,evtmgr->FindEvent(LOGIN_HEAD)){
            p->recv(203,QVariantMap());
        }
        break;
    }
}

void menu::rank_finished(QStringList l)
{
    sendevt(9,QVariantMap());
}
