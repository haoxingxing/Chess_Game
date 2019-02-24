#include "jsoncoder.h"
#include "ranking.h"
#include "ui_ranking.h"

Ranking::Ranking(QVariantMap map,QWidget *parent,MainNetworkManger* mnm) :
    WindowProcessSlot ("rank",parent,mnm),
    ui(new Ui::Ranking)
{
    ui->setupUi(this);
    all=map.value("numbr").toInt();
    ui->nbs->setText(QString::number(0)+"/"+QString::number(all));
    ui->status->setText("Waiting");
    connect(ntwkmgrr,&MainNetworkManger::Message,this,&Ranking::recv);
}

Ranking::~Ranking()
{
    delete ui;
}

void Ranking::recv(QVariantMap map)
{
    if (map.value("status")==100)
    {
            ui->status->setText("The Rank List is Fulled,Retrying");
            ntwkmgrr->send(Jsoncoder::encode(QVariantMap({
                                                             FM
                                                          std::make_pair("type","rank"),
                                                          std::make_pair("numbers",all),
                                                         })));
    }

    ui->nbs->setText(QString::number(map.value("nnb").toInt())+"/"+QString::number(all));
    ui->plrlst->setText("\""+map.value("list").toStringList().join("\",\"")+"\"");
}
void Ranking::on_exit_clicked()
{
    ntwkmgrr->send(Jsoncoder::encode(QVariantMap({
                                                  For("rank")
                                                  std::make_pair("action","exit"),
                                              })));
}
