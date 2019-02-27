#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "mainnetworkmanger.h"
#include "windowprocessslot.h"
namespace Ui {
class menu;
}

class menu : public WindowProcessSlot
{
    Q_OBJECT

public:
    explicit menu(MainNetworkManger *ntmgrr,QWidget *parent = nullptr,QVariantMap startup_map = QVariantMap());
    ~menu();
private slots:
    void on_start_clicked();
    void on_logout_clicked();
    void on_maker_clicked();
private:
void recv(QVariantMap);
    Ui::menu *ui;
};

#endif // MENU_H
