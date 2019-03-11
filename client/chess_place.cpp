#include "chess_place.h"
#include "jsoncoder.h"
#include "ui_chess_place.h"

#include <QPaintEvent>
#include <QMessageBox>
chess_place::chess_place(MainNetworkManger *ntwkmgr,QWidget *parent,int x,int y,QStringList plrlst) :
    WindowProcessSlot ("chess_place",parent,ntwkmgr),
    xlen(x),
    ylen(y),
    ui(new Ui::chess_place),
    plrlst(plrlst)
{
    ui->setupUi(this);
    ui->exit->hide();

    connect(ui->exit,&QPushButton::clicked,this,[&]{
        this->hide();
        this->deleteLater();
        emit closed();
    });
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("request","chess_place"),
                                   std::make_pair("x",25),
                                   std::make_pair("y",25),
                               }),MAIN_HEAD);
}

chess_place::~chess_place()
{
    delete ui;
}


void chess_place::recv(QVariantMap map)
{
    switch (map.value("status").toInt()) {
    case 200:
        info=new chess_place_info(this);
        info->show();
        info->AppenedMsg("Waiting For Server");
        chat_window=new chat(ntwkmgrr,this);
        chat_window->join(plrlst.join("@"));
        chat_window->show();
        this->sendClientReady(plrlst);
        break;
    case 201:
        info->AppenedMsg("Server Ready");
        info->AppenedMsg("Loading Chesses");
        this->my_username=map.value("usn").toString();
        if (my_username==map.value("black")){
            my=BLACK;
            enm=WHITE;
        }
        else{
            my=WHITE;
            enm=BLACK;
        }
        init_chesses();
        info->AppenedMsg("DONE!");
        this->activateWindow();
        break;
    case 202:
        info->AppenedMsg("Set Chess:");
        info->AppenedMsg("x: "+QString::number(map.value("x").toInt()));
        info->AppenedMsg("y: "+QString::number(map.value("y").toInt()));
        info->AppenedMsg("By: "+map.value("who").toString());
        set_chess_color(map.value("x").toInt(),map.value("y").toInt(),(map.value("who")==my_username)?my:enm);
        this->activateWindow();
        break;
    case 203:
        info->AppenedMsg("Game Over: ""Your competitor has been offline");
        ui->exit->setText("Game Over: ""Your competitor has been offline\nExit");
        //QMessageBox::information(this,"Game Over","Your competitor has been offline",QMessageBox::Ok);
        goto close;
    case 204:
        ui->exit->setText("Game Over: ""Your competitor has been offline\nExit");
        //QMessageBox::information(this,"Game Over","Server Force Close",QMessageBox::Ok);
        goto close;
    case 205:
        info->AppenedMsg("Game Over: "+map.value("winner").toString()+" is the winner");
        ui->exit->setText("Game Over: "+map.value("winner").toString()+" is the winner\nExit");
        //QMessageBox::information(this,"Game Over",map.value("winner").toString()+" is the winner",QMessageBox::Ok);
        goto close;
    }
    if (hasinitted){
        if (map.value("nowhosturn").toString()==my_username)
            ismyturn=true;
        else
            ismyturn=false;
        info->AppenedMsg("Now is "+(!ismyturn?map.value("nowhosturn").toString()+"'s":"your")+" turn");
        turn_chesses_enable(ismyturn);
    }
    return;
close:
    this->activateWindow();
    this->delete_chesses();
    repaint();
    ui->exit->show();
}

void chess_place::turn_chess_enable(int x, int y, bool isenable)
{
    if (hasinitted)
    {
        if (!chesses[x][y]->iscovered)
            chesses[x][y]->pushbutton->setEnabled(isenable);
        chesses[x][y]->pushbutton->show();
    }
}

void chess_place::turn_chesses_enable(bool isenable)
{
    for (int x=0;x<=xlen;++x)
    {
        for (int y=0;y<=ylen;++y)
        {
            turn_chess_enable(x,y,isenable);
        }
    }
}

void chess_place::init_chesses()
{
    for (int x=0;x<=xlen;++x)
    {
        chesses.push_back(QVector<chess_place::chess*>());
        for (int y=0;y<=ylen;++y)
        {
            chesses[x].push_back(new chess(this));
            connect(chesses[x][y]->pushbutton,&QPushButton::clicked,this,[=]{
                this->sendChessDropped(x,y);
            });
        }
    }
    this->setWindowTitle(QString("Chess: "+my_username+" - "+QString((my==WHITE)?("白方"):("黑方"))));//添加窗口标题提示信息
    hasinitted=true;
}

void chess_place::delete_chesses()
{
    hasinitted=false;
    for (int x=0;x<=xlen;++x)
    {
        for (int y=0;y<=ylen;++y)
        {
            if (!chesses[x][y]->iscovered)
                delete chesses[x][y]->pushbutton;
        }
    }
}

void chess_place::sendClientReady(QStringList PLRLST)
{
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("status",101),
                                   std::make_pair("players",PLRLST)
                               }),CHESS_HEAD);
    info->AppenedMsg("client ready");
}

void chess_place::sendChessDropped(int x, int y)
{
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("status",102),
                                   std::make_pair("x",x),
                                   std::make_pair("y",y),
                               }),CHESS_HEAD);
    info->AppenedMsg("Request has sent");
}

void chess_place::set_chess_color(int x, int y, chess_place::colors cls)
{
    if (hasinitted)
    {
        QString sht;
        switch (cls) {
        case WHITE:
            sht="background-color: rgb(255, 255, 255);border-radius:8px;";
            break;
        case BLACK:
            sht="background-color: rgb(0, 0, 0);border-radius:8px;";//修改按钮外形控件，实现圆形棋子
            break;
        }
        turn_chess_enable(x,y,false);
        chesses[x][y]->iscovered=true;
        chesses[x][y]->pushbutton->setStyleSheet(sht);
        //chesses[x][y]->pushbutton->setAutoFillBackground(true);
        chesses[x][y]->pushbutton->show();
    }
}


void chess_place::paintEvent(QPaintEvent*)
{
    if (this->width()!=this->height())
    {
        this->resize(this->height(),this->height());
    }
    QPainter ptr(this);
    int screen_wid=this->width();
    int screen_hei=this->height();
    if (!(screen_hei>=ylen*10&&screen_wid>=xlen*10))
        resize(xlen*30,ylen*30);
    if (hasinitted)
    {
        per_ge_wid=screen_wid / xlen;
        per_ge_hei=screen_hei / ylen;
        screen_left_wid=screen_wid % per_ge_wid;
        screen_left_hei=screen_hei % per_ge_hei;
        screen_left_left_right=screen_left_wid/2;
        screen_left_up_down=screen_left_hei/2;
        for (int i=0;i<=xlen;++i) {
            ptr.drawLine(screen_left_left_right+per_ge_wid*i,screen_left_up_down,screen_left_left_right+per_ge_wid*i,screen_left_up_down+per_ge_hei*ylen);
        }
        for (int i=0;i<=ylen;++i) {
            ptr.drawLine(screen_left_left_right,screen_left_up_down+per_ge_hei*i,screen_left_left_right+per_ge_wid*xlen,screen_left_up_down+per_ge_hei*i);
        }
        for (int x=0;x<=xlen;++x)
        {
            for (int y=0;y<=ylen;++y)
            {
                chesses[x][y]->pushbutton->setGeometry((screen_left_left_right+per_ge_wid*x-(per_ge_wid/2))+(per_ge_wid/5),(screen_left_up_down+per_ge_hei*y-(per_ge_hei/2))+(per_ge_hei/5),per_ge_wid/3*2,per_ge_hei/3*2);

                chesses[x][y]->pushbutton->show();
            }
        }
    }
    ui->exit->resize(this->width(),this->height());
}



chess_place::chess::chess(chess_place* window){
    pushbutton=new QPushButton("",window);
    pushbutton->setFlat(true);
    pushbutton->show();

}
