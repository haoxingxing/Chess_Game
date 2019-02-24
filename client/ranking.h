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
    explicit Ranking(QVariantMap startup,QWidget *parent = nullptr,MainNetworkManger* mnm = nullptr);
    ~Ranking();
public slots:
    void recv(QVariantMap);
private slots:
    void on_exit_clicked();

private:
    Ui::Ranking *ui;
    int all;
};

#endif // RANKING_H
