#include "menu.h"
#include "ui_menu.h"
#include "jsoncoder.h"
#include "chess_place.h"
#include <QMessageBox>
menu::menu(MainNetworkManger *ntmgrr,QWidget *parent,QString username) :
    WindowProcessSlot ("menu",parent,ntmgrr),
    ui(new Ui::menu)
{
    ui->setupUi(this);
    ui->login_status->setText(ui->login_status->text() + username);
}


menu::~menu()
{
    delete ui;
}


void menu::on_start_clicked()
{
    ui->start->setText("Loading");
    ui->start->setEnabled(false);
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("request","rank"),
                                   std::make_pair("numbers",2),
                                   std::make_pair("rank_info","25*25"),
                               }),MAIN_HEAD);

}

void menu::recv(QVariantMap mp)
{
    switch(mp.value("status").toInt()){
        case 102:
        hd=new Ranking(ntwkmgrr,nullptr,2);
        hd->show();
        connect(hd,&Ranking::fulled,this,&menu::Lets_go_to_the_warplace_with_the_player_list);
        this->hide();
        ui->start->setEnabled(true);
        ui->start->setText("Start");
        break;
    case 103:
        this->show();

    }
}

void menu::on_logout_clicked()
{
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("type",303)
                               }),LOGIN_HEAD);
}

void menu::on_maker_clicked()
{
    QMessageBox::information(this,"Maker","该软件由“ 郝星星 ”设计制作\nThe Software is made by haoxingxing\nPowered by Qt and C++");
}

void menu::Lets_go_to_the_warplace_with_the_player_list(QStringList list)
{

    chess_place* hd=new chess_place(ntwkmgrr,nullptr,25,25,list);
    connect(hd,&chess_place::closed,this,&menu::show);
    hd->show();
}
