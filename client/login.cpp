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
    connect(ntwkmgrr,&MainNetworkManger::Message,this,&login::recv);
}

login::~login()
{
    delete ui;
}

void login::Process_Login(LOGIN_TYPE tp)
{
    ntwkmgrr->send(Jsoncoder::encode(QVariantMap({
                                                     FM
                                                     std::make_pair("username",ui->user->text()),
                                                     std::make_pair("password",QString("%1").arg(QString(QCryptographicHash::hash((ui->pwd->text()+"Client_First_Slat_+++").toUtf8(),QCryptographicHash::Md5).toHex()))),
                                                     std::make_pair("type",(tp==LOGIN)?"login":"register")
                                                 })));
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
    if (!map.value("is_success").toBool())
    {
        ui->emsg->setText(map.value("emsg").toString());
        ui->emsg->show();
        ui->logi->setEnabled(true);
        ui->reg->setEnabled(true);
        ui->user->setEnabled(true);
        ui->pwd->setEnabled(true);
        ui->pwd->clear();
    }
}

void login::on_logi_clicked()
{
    Process_Login(LOGIN);

}
