//#include "jsoncoder.h"
//#include "ranking.h"
//#include "ui_ranking.h"

//Ranking::Ranking(MainNetworkManger* mnm,QWidget *parent,int nb) :
//    WindowProcessSlot ("rank",parent,mnm),
//    ui(new Ui::Ranking),
//    all(nb)
//{
//    ui->setupUi(this);
//    ui->nbs->setText(QString::number(1)+"/"+QString::number(all));
//    ui->status->setText("Joining");
//    this->SendJoin();
//}

//Ranking::~Ranking()
//{
//    delete ui;
//}

//void Ranking::recv(QVariantMap map)
//{
//    switch (map.value("status").toInt())
//    {
//    case 100:
//        ui->status->setText("The Rank List is Fulled,Retrying");
//        this->SendJoin();
//        break;
//    case 101:
//        emit fulled(map.value("players").toStringList());
//        this->dscnktd();
//        break;
//    case 103:
//        ui->nbs->setText(QString::number(map.value("nnb").toInt())+"/"+QString::number(all));
//        ui->plrlst->setText("\""+map.value("list").toStringList().join("\",\"")+"\"");
//        break;
//    case 104:
//        ui->status->setText("Joined * Waiting");
//        break;
//    case 105:
//        this->dscnktd();
//    }


//}
//void Ranking::on_exit_clicked()
//{
//    ntwkmgrr->send(QVariantMap({
//                                   std::make_pair("status",101),
//                               }),RANK_HEAD);
//}

//void Ranking::SendJoin()
//{
//    ntwkmgrr->send(QVariantMap({
//                                   std::make_pair("status",102),
//                               }),RANK_HEAD);
//}
