#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "mainnetworkmanger.h"
#include "windowprocessslot.h"
#include "menu.h"
namespace Ui {
class login;
}

class login : public WindowProcessSlot
{
    Q_OBJECT

public:
    explicit login(MainNetworkManger *ntmgr,QWidget *parent = nullptr);
    ~login();
    enum LOGIN_TYPE{
        LOGIN,
        REGISTER
    };
    void Process_Login(LOGIN_TYPE);
private slots:
    void on_reg_clicked();
    void recv(QVariantMap);
    void on_logi_clicked();
private:
    menu *hd = nullptr;
    Ui::login *ui;
};

#endif // LOGIN_H
