#include "login.h"
#include "ui_login.h"
#include "jsoncoder.h"
#include <QCryptographicHash>
login::login(MainNetworkManger *ntmgr,QWidget *parent) :
    WindowProcessSlot("login",parent,ntmgr),
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
    ntwkmgrr->send(QVariantMap({
                                   std::make_pair("username",ui->user->text()),
                                   std::make_pair("password",QString("%1").arg(QString(QCryptographicHash::hash((ui->pwd->text()+"Client_First_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex()))),
                                   std::make_pair("type",(tp==LOGIN)?301:302)
                               }),LOGIN_HEAD);
    ui->user->setEnabled(false);
    ui->pwd->setEnabled(false);
    ui->logi->setEnabled(false);
    ui->reg->setEnabled(false);
}

void login::on_reg_clicked()
{
    Process_Login(REGISTER);
}

void login::recv(QVariantMap map)
{
    switch(map.value("status").toInt())
    {
    case 301:
        ui->emsg->setText(map.value("emsg").toString());
        ui->emsg->show();
        ui->logi->setEnabled(true);
        ui->reg->setEnabled(true);
        ui->user->setEnabled(true);
        ui->pwd->setEnabled(true);
        ui->pwd->clear();
        break;
    case 302:
        ui->pwd->clear();
        hd = new menu(ntwkmgrr,nullptr,map.value("username").toString());
        hd->show();
        this->hide();
        break;
    case 303:
        if (hd!=nullptr){
            hd->hide();
            delete hd;
            hd=nullptr;
        }
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
