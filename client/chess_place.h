#ifndef CHESS_PLACE_H
#define CHESS_PLACE_H

#include <QWidget>
#include "windowprocessslot.h"
#include "mainnetworkmanger.h"
#include "chess_place_info.h"
#include <QPainter>
#include <QPushButton>
namespace Ui {
class chess_place;
}

class chess_place : public WindowProcessSlot
{
    Q_OBJECT

public:
    explicit chess_place(MainNetworkManger *ntwkmgr,QWidget *parent,int x,int y,QStringList);
    ~chess_place();

signals:
    void closed();
private slots:
    void recv(QVariantMap);
private:
    struct chess{
    public:
        chess(chess_place*);
        QString whose;
        bool iscovered = false;
        QPushButton* pushbutton;
    };
    enum colors{
        WHITE,
        BLACK
    };
    void turn_chess_enable(int x,int y,bool isenable);
    void turn_chesses_enable(bool isenable);
    void set_chess_color(int x,int y,colors);
    void init_chesses();
    void delete_chesses();

    void sendClientReady(QStringList);
    void sendChessDropped(int x,int y);

    void paintEvent(QPaintEvent*);
    int xlen,ylen;
    bool hasinitted=false;
    Ui::chess_place *ui;
    QString my_username,enm_username;
    QVector<QVector<chess*>> chesses;
    colors my,enm;
    bool ismyturn;
    QStringList plrlst;

    chess_place_info *info;
};

#endif // CHESS_PLACE_H
