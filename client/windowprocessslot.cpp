//#include "windowprocessslot.h"

//WindowProcessSlot::WindowProcessSlot(QString name,QWidget *parent,MainNetworkManger* ntwkmgr) : QWidget(parent) {
//    ntwkmgrr=ntwkmgr;
//    this->name=name;
//    connect(ntwkmgrr,&MainNetworkManger::Disconnect,this,&WindowProcessSlot::dscnktd);
//    connect(ntwkmgrr,&MainNetworkManger::Message,this,&WindowProcessSlot::recv_t);
//}

//void WindowProcessSlot::dscnktd() {
//    this->hide();
//    this->deleteLater();
//}

//void WindowProcessSlot::recv_t(QVariantMap map)
//{
//    if (map.value("for").toString().contains(name))
//    {
//        this->recv(map);
//        if (map.value("hide").toString().contains(name)||map.value("hide").toString()=="all")
//            this->hide();
//        if (map.value("show").toString().contains(name)||map.value("show").toString()=="all")
//            this->show();
//        if (map.value("close").toString().contains(name)||map.value("close").toString()=="all")
//        {
//            this->hide();
//            this->deleteLater();
//        }
//        if (map.contains("stylesheet"))
//        {
//            this->setStyleSheet(map.value("stylesheet").toString());
//        }
//    }
//}
