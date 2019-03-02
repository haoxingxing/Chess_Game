#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "mainnetworkmanger.h"
#include "windowprocessslot.h"
#include "ranking.h"
namespace Ui {
class menu;
}

class menu : public WindowProcessSlot
{
    Q_OBJECT

public:
    explicit menu(MainNetworkManger *ntmgrr,QWidget *parent = nullptr,QString username = "undefined");
    ~menu();
private slots:
    void on_start_clicked();
    void on_logout_clicked();
    void on_maker_clicked();
    void Lets_go_to_the_warplace_with_the_player_list(QStringList);
    void recv(QVariantMap);
private:
    Ranking* hd = nullptr;
    Ui::menu *ui;
};

#endif // MENU_H
