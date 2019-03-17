#include "eventwidget.h"

EventWidget::EventWidget(QWidget *parent,MainNetworkManger* ntwkmgr,QString evid) :
    QWidget(parent),Event (ntwkmgr,evid)
{
}

EventWidget::~EventWidget()
{
}

void EventWidget::SetWindowInfo(QVariantMap args)
{
    /*
     * Args:
     * {
     *      "height":int
     *      "width":int
     *      "title":QString
     *      "sheetstyle":QString
     *      "children":enum{
     *                  LOGIN,....
     *                  }
     */
    this->resize((args.contains("height"))?args["height"].toInt():this->height(),(args.contains("height"))?args["width"].toInt():this->width());
    this->setWindowTitle(args.contains("title")?args["title"].toString():this->windowTitle());
    this->setStyleSheet(args["stylesheet"].toString());

}
