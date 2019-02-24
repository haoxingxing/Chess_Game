#ifndef CHESS_PLACE_H
#define CHESS_PLACE_H

#include <QWidget>
#include "windowprocessslot.h"
#include "mainnetworkmanger.h"
#include <QPainter>
#include <QPushButton>
namespace Ui {
class chess_place;
}

class chess_place : public WindowProcessSlot
{
    Q_OBJECT

public:
    explicit chess_place(MainNetworkManger *ntwkmgr,QWidget *parent = nullptr,QVariantMap map = QVariantMap());
    ~chess_place();

private slots:
    void recv(QVariantMap);
    void dropchess(int x,int y);
private:
    void paintEvent(QPaintEvent *event);
    QPushButton* getAnewChess();
    void init_chesses();
    void get_all_chesses();
    void turn_chess_off();
    void turn_chess_on();
    void drp(int x,int y,bool bow);
    Ui::chess_place *ui;
    int pan_hei;
    int pan_wid;
    QVector<QVector<QPushButton*>> chesses;
    QVector<QVector<bool>> chesses_status;
    QString enm;
    bool isgaming=false;
    bool black_or_white;
    bool isyourturn=false;
};

#endif // CHESS_PLACE_H
