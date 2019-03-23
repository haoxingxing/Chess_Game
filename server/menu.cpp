#include "menu.h"
#include "eventmanger.h"
#include "file_codes.h"
#include "ranking.h"
Menu::Menu(MainNetworkManger *rp,QString evid,EventManger* e):Event(MENU_HEAD,rp,evid,e){

}

void Menu::recv(const int & s, const QVariantMap &)
{
    switch (s) {
    case 301:
    {
        Event* e=evtmgr->GetEvent(evtmgr->NewEvent(EventManger::event_types::rank));
        e->recv(401,File_Codes::readmap("menu_rank_1"));
        sendevt(9,QVariantMap());
        e->exec();        
        sendevt(8,QVariantMap());
                
        break;
    }
    case 302:
        foreach(Event* p,evtmgr->FindEvent(LOGIN_HEAD)){
            p->recv(203,QVariantMap());
        }
        close();
        break;
    }
}
