#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include "windowprocessslot.h"
namespace Ui {
class chat;
}

class chat : public WindowProcessSlot
{
    Q_OBJECT
public:
    explicit chat(MainNetworkManger* ntwkmgr,QWidget *parent);
    ~chat();
    void appenedmessage(QString msg);
    void join(QString cn);
    void exit(QString cn);
private slots:
    void on_commit_clicked();

private:
    void recv(QVariantMap);
    Ui::chat *ui;
    QString cn;
    bool isready = false;
};

#endif // CHAT_H
