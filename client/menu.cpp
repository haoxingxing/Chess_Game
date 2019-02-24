#include "menu.h"
#include "ui_menu.h"
#include "jsoncoder.h"
#include <QMessageBox>
menu::menu(MainNetworkManger *ntmgrr,QWidget *parent,QVariantMap startup_map) :
    WindowProcessSlot ("menu",parent,ntmgrr),
    ui(new Ui::menu)
{
    ui->setupUi(this);

//    connect(ntwkmgrr,&MainNetworkManger::Message,this,&menu::recv);
    ui->login_status->setText(ui->login_status->text() + startup_map.value("username").toString());
}


menu::~menu()
{
    delete ui;
}


void menu::on_start_clicked()
{
    ui->start->setText("Loading");
    ui->start->setEnabled(false);
    ntwkmgrr->send(Jsoncoder::encode(QVariantMap({
                                                     FM
                                                  std::make_pair("type","rank"),
                                                  std::make_pair("numbers",2),
                                                  std::make_pair("rank_info","25*25")
                                                 })));
}

//void menu::recv(QVariantMap mp)
//{

//}

void menu::on_logout_clicked()
{
    ntwkmgrr->send(Jsoncoder::encode(QVariantMap({
                                                        FM
                                                     std::make_pair("type","logout")
                                              })));
}

void menu::on_maker_clicked()
{
    QMessageBox::information(this,"Maker","The Software is made by haoxingxing\nPowered by Qt and C++");
}
