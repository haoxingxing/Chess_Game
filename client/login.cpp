#include "login.h"
#include "ui_login.h"
#include "jsoncoder.h"
#include <QCryptographicHash>
login::login(MainNetworkManger *ntmgr,QWidget *parent,QString evid) :
    EventWidget(parent,ntmgr,evid),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->emsg->hide();
}

login::~login()
{
    delete ui;
}

void login::Process_Login(LOGIN_TYPE tp)
{
    this->send((tp==LOGIN)?201:202,QVariantMap({
                                                   std::make_pair("username",ui->user->text()),
                                                   std::make_pair("password",QString("%1").arg(QString(QCryptographicHash::hash((ui->pwd->text()+"Client_First_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex()))),
                                               }));
    ui->user->setEnabled(false);
    ui->pwd->setEnabled(false);
    ui->logi->setEnabled(false);
    ui->reg->setEnabled(false);
}

void login::err(int, QString e)
{
    ui->emsg->setText(e);
    ui->emsg->show();
    ui->logi->setEnabled(true);
    ui->reg->setEnabled(true);
    ui->user->setEnabled(true);
    ui->pwd->setEnabled(true);
    ui->pwd->clear();
}

void login::on_reg_clicked()
{
    Process_Login(REGISTER);
}

void login::recv(int status,QVariantMap map)
{
    switch(status)
    {
    case 1:
        ui->pwd->clear();
//        hd = new menu(ntwkmgrr,nullptr,map.value("username").toString());
//        hd->show();
        this->hide();
        ntwkmgr->username=map["username"].toString();
        break;
    case 2:
//        if (hd!=nullptr){
//            hd->hide();
//            delete hd;
//            hd=nullptr;
//        }
        ui->logi->setEnabled(true);
        ui->reg->setEnabled(true);
        ui->user->setEnabled(true);
        ui->pwd->setEnabled(true);
        ui->pwd->clear();
        ui->emsg->hide();
        ui->emsg->clear();
        this->show();
        break;
    }
}

void login::on_logi_clicked()
{
    Process_Login(LOGIN);
}
