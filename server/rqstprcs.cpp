#include "rqstprcs.h"

RQSTPRCS::RQSTPRCS(QString name,RequestProcesser *parent) : QObject(parent),ntwkmgr(parent),class_name(name)
{
    connect(parent,&RequestProcesser::Message,this,&RQSTPRCS::recv_t);
    connect(parent,&RequestProcesser::dscnktd,this,&RQSTPRCS::dscnktd);
}

void RQSTPRCS::recv_t(QVariantMap map)
{
    if (map.value("for").toString().contains(class_name))
        this->recv(map);
}

void RQSTPRCS::ChangeColor(int r, int g, int b)
{
    ntwkmgr->send(QVariantMap({
                                std::make_pair("stylesheet","background-color: rgb("+QString::number(r)+", "+QString::number(g)+", "+QString::number(b)+");")
                              }),class_name);
}
