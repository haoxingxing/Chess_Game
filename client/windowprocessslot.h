#ifndef WINDOWPROCESSSLOT_H
#define WINDOWPROCESSSLOT_H

#include <QWidget>
#include <mainnetworkmanger.h>
class WindowProcessSlot : public QWidget
{
    Q_OBJECT
public:
    explicit WindowProcessSlot(QString name,QWidget *parent = nullptr,MainNetworkManger* ntwkmgr = nullptr);
    MainNetworkManger *ntwkmgrr;
signals:  
    void shide();
    void sshow();
    void sdel();

public slots:
    virtual void dscnktd();
     void recv_t(QVariantMap);
protected:
    QString name;
};

#endif // WINDOWPROCESSSLOT_H
