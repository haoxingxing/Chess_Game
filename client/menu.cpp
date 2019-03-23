#include "menu.h"
#include "ui_menu.h"
#include "jsoncoder.h"
#include "chess_place.h"
#include <QMessageBox>
#include "mainnetworkmanger.h"
menu::menu(MainNetworkManger *ntmgr,QWidget *parent,QString evid) :
    EventWidget (parent,ntmgr,evid),
    ui(new Ui::menu)
{
    ui->setupUi(this);
    ui->login_status->setText("Hi! "+ntwkmgr->username);
}


menu::~menu()
{
    delete ui;
}


void menu::on_start_clicked()
{
    ui->start->setText("Loading");
    ui->start->setEnabled(false);
    send(301,QVariantMap());

}

void menu::recv(int s,QVariantMap)
{
    switch(s){
    case 8:
        this->hide();
        ui->start->setEnabled(true);
        ui->start->setText("Start");
        break;
    case 9:
        this->show();
    }
}

void menu::on_logout_clicked()
{
    send(302,QVariantMap());

}

void menu::on_maker_clicked()
{
    QMessageBox::information(this,"Maker","该软件由“ 郝星星 ”设计制作\nThe Software is made by haoxingxing\nPowered by Qt and C++");
}
