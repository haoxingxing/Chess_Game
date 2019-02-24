#include "chess_place.h"
#include "jsoncoder.h"
#include "ui_chess_place.h"

chess_place::chess_place(MainNetworkManger *ntwkmgr,QWidget *parent,QVariantMap map) :
    WindowProcessSlot ("chess_place",parent,ntwkmgr),
    ui(new Ui::chess_place)
{
    ui->setupUi(this);
    pan_hei=map.value("rank_info").toString().split("*")[0].toInt();
    pan_wid=map.value("rank_info").toString().split("*")[1].toInt();
    isyourturn=true;
    black_or_white=true;
    isgaming=true;
    enm=map.value("players").toStringList()[1];
    this->init_chesses();
    this->repaint();
//    for (int i=0;i<map.value("players").toStringList().length();++i)
//        if (map.value("players").toStringList()[0]==map.value("you").toString()){
//            enm=map.value("players").toStringList()[1];
//            black_or_white=true;
//            isyourturn=true;
//        } else {
//            enm=map.value("players").toStringList()[0];
//            black_or_white=false;
//            isyourturn=false;
//        }
//    if (!(pan_hei>0&&pan_wid>0))
//        this->dscnktd();
//    connect(ntwkmgrr,&MainNetworkManger::Message,this,&chess_place::recv);
}

chess_place::~chess_place()
{
    delete ui;
}

void chess_place::recv(QVariantMap map)
{
    switch (map.value("status").toInt()) {
    case 201:
        isgaming=true;
        this->init_chesses();
        if (isyourturn)
            turn_chess_on();
        else {
            turn_chess_off();
        }
        this->repaint();
        break;
    case 202:
        drp(map.value("x").toInt(),map.value("y").toInt(),(map.value("who")==enm)?!black_or_white:black_or_white);
        if (map.value("who")==enm){
            isyourturn=true;
            turn_chess_on();
        }
        else {
            isyourturn=false;
            turn_chess_off();
        }
        break;
    case 203:
        for (int x=0;x<=pan_wid;++x)
            for (int y=0;y<=pan_hei;++y)
                if (map.value(QString::number(x)+"*"+QString::number(y))!="")
                    drp(x,y,map.value(QString::number(x)+"*"+QString::number(y))==enm?!black_or_white:black_or_white);
        break;
    }
}

void chess_place::dropchess(int x, int y)
{
    if (isgaming)
    {
        ntwkmgrr->send(Jsoncoder::encode(QVariantMap({
                                                         For("chess_place")
                                                         std::make_pair("action","drop"),
                                                         std::make_pair("x",x),
                                                         std::make_pair("y",y)
                                                     })));
    }
}

void chess_place::paintEvent(QPaintEvent*)
{
    if (isgaming)
    {
        QPainter ptr(this);
        int screen_wid=this->width();
        int screen_hei=this->height();
        if (!(screen_hei>=pan_hei*10&&screen_wid>=pan_wid*10))
            resize(pan_wid*10,pan_hei*10);
        int per_ge_wid=screen_wid / pan_wid;
        int per_ge_hei=screen_hei / pan_hei;
        int screen_left_wid=screen_wid % per_ge_wid;
        int screen_left_hei=screen_hei % per_ge_hei;
        int screen_left_left_right=screen_left_wid/2;
        int screen_left_up_down=screen_left_hei/2;
        for (int i=0;i<=pan_wid;++i) {
            ptr.drawLine(screen_left_left_right+per_ge_wid*i,screen_left_up_down,screen_left_left_right+per_ge_wid*i,screen_left_up_down+per_ge_hei*pan_hei);
        }
        for (int i=0;i<=pan_hei;++i) {
            ptr.drawLine(screen_left_left_right,screen_left_up_down+per_ge_hei*i,screen_left_left_right+per_ge_wid*pan_wid,screen_left_up_down+per_ge_hei*i);
        }
        for (int x=0;x<=pan_wid;++x)
            for(int y=0;y<=pan_hei;++y)
            {
                chesses[x][y]->setGeometry(screen_left_left_right+per_ge_wid*x-(per_ge_wid/2),screen_left_up_down+per_ge_hei*y-(per_ge_hei/2),per_ge_wid,per_ge_hei);
            }
    }
}
QPushButton* chess_place::getAnewChess()
{
    QPushButton* pb=new QPushButton("",this);
    pb->setFlat(true);
    pb->setText("0");
    pb->setGeometry(0,0,0,0);
    //pb->setAutoFillBackground(true);
    return pb;
}

void chess_place::init_chesses()
{
    for (int x=0;x<=pan_wid;++x)
    {
        chesses.insert(x,QVector<QPushButton*>());
        chesses_status.insert(x,QVector<bool>());
        for(int y=0;y<=pan_hei;++y)
        {
            chesses_status[x].insert(y,false);
            chesses[x].insert(y,getAnewChess());
            //            connect(chesses[x][y],&QPushButton::clicked,this,[&]{
            //                this->dropchess(x,y);
            //            });
        }
    }
}

void chess_place::get_all_chesses()
{
    ntwkmgrr->send(Jsoncoder::encode(QVariantMap({
                                                     For("chess_place")
                                                     std::make_pair("action","get_all")
                                                 })));
}

void chess_place::turn_chess_off()
{
    for (int x=0;x<=pan_wid;++x)
        for(int y=0;y<=pan_hei;++y)
        {
            if (!chesses_status[x][y])
            {
                chesses[x][y]->setEnabled(false);
            }
        }
}

void chess_place::turn_chess_on()
{
    for (int x=0;x<=pan_wid;++x)
        for(int y=0;y<=pan_hei;++y)
        {
            if (!chesses_status[x][y])
            {
                chesses[x][y]->setEnabled(true);
            }
        }
}

void chess_place::drp(int x, int y, bool bow)
{
    chesses[x][y]->setEnabled(false);
    if (bow){
        chesses_status[x][y]=true;
        chesses[x][y]->setStyleSheet("background-color: rgb(0, 0, 0);");
    } else {
        chesses_status[x][y]=true;
        chesses[x][y]->setStyleSheet("background-color: rgb(255, 255, 255);");
    }
}

