#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "mainnetworkmanger.h"
#include "eventwidget.h"
#include "ranking.h"
namespace Ui {
class menu;
}

class menu : public EventWidget
{
    Q_OBJECT

public:
    explicit menu(MainNetworkManger *ntmgr,QWidget *parent,QString evid);
    ~menu();
private slots:
    void on_start_clicked();
    void on_logout_clicked();
    void on_maker_clicked();
    void recv(int,QVariantMap);
private:
    Ui::menu *ui;
};

#endif // MENU_H
