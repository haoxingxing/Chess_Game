#ifndef RANKING_H
#define RANKING_H

#include <QWidget>
#include "windowprocessslot.h"
namespace Ui {
class Ranking;
}

class Ranking : public WindowProcessSlot
{
    Q_OBJECT

public:
    explicit Ranking(MainNetworkManger* mnm = nullptr,QWidget *parent = nullptr,int allnbers=0);
    ~Ranking();
public slots:
    void recv(QVariantMap);
signals:
    void fulled(QStringList);
private slots:
    void on_exit_clicked();

private:
    void SendJoin();
    Ui::Ranking *ui;
    int all;
};

#endif // RANKING_H
