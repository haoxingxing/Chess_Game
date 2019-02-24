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

private:
    void paintEvent(QPaintEvent *event);
    QPushButton* getAnewChess();
    void init_chesses();
    Ui::chess_place *ui;
    int pan_hei;
    int pan_wid;
    QVector<QVector<QPushButton*>> chesses;
};

#endif // CHESS_PLACE_H
