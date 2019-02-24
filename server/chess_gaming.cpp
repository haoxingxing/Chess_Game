#include "chess_gaming.h"
#include "jsoncoder.h"

chess_gaming::chess_gaming(int pan_w,int pan_h,QObject *parent,RequestProcesser* mnm) :
    QObject(parent),
    ntwkmgr(mnm),
    pan_w(pan_w),
    pan_h(pan_h)
{
    this->SendStartInfo();
}

void chess_gaming::SendStartInfo()
{
    ntwkmgr->send(Jsoncoder::encode(QVariantMap({
                                           std::make_pair("to","chess_place"),
                                           std::make_pair("hide","menu"),
                                           std::make_pair("wid",pan_w),
                                           std::make_pair("hei",pan_h),
                                           std::make_pair("enm","menu")
                                                })));
}

void chess_gaming::recv_process(QVariantMap map)
{

}
