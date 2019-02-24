#include "chess_place.h"
#include "ui_chess_place.h"

chess_place::chess_place(MainNetworkManger *ntwkmgr,QWidget *parent,QVariantMap map) :
    WindowProcessSlot ("chess_place",parent,ntwkmgr),
    ui(new Ui::chess_place)
{
    ui->setupUi(this);
    pan_hei=map.value("rank_info").toString().split("*")[0].toInt();
    pan_wid=map.value("rank_info").toString().split("*")[1].toInt();
    if (!(pan_hei>0&&pan_wid>0))
        this->dscnktd();
    this->init_chesses();
    resize(640,640);
}

chess_place::~chess_place()
{
    delete ui;
}

void chess_place::paintEvent(QPaintEvent*)
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
QPushButton* chess_place::getAnewChess()
{
    QPushButton* pb=new QPushButton("",this);
    pb->setFlat(true);
    pb->setGeometry(0,0,0,0);
    return pb;
}

void chess_place::init_chesses()
{

    for (int x=0;x<=pan_wid;++x)
    {
        chesses.insert(x,QVector<QPushButton*>());
        for(int y=0;y<=pan_hei;++y)
        {
            chesses[x].insert(y,getAnewChess());
        }
    }
}
