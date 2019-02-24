#ifndef RANKING_H
#define RANKING_H

#include <QObject>
#include "requestprocesser.h"
#include <QTimer>

class rank_node : public QObject
{
    Q_OBJECT
public:
    struct Rank_Node {
        int now;
        QStringList user_list;
        int fulledSiganlSended = 0;
    };
    explicit rank_node(QObject *parent = nullptr,int n = 2,QString rank_info = "");
    bool join(QString username);
    void check_full();
    void del(QString username);
signals:
    void fulled(QStringList);
    void number_changed(int,QStringList);
private slots:
    void check_number_changed();
private:

    static QMap<QString,Rank_Node> rn;
    int nb;
    QString rkif;
    bool isranking = false;
    QTimer tm_ch_sg;
    int now_in_this_class;
};

class ranking : public QObject
{
    Q_OBJECT
public:
    explicit ranking(QObject *parent = nullptr,RequestProcesser* mnm = nullptr,int n=2,QString username="undefine",QString rank_info = "",QString to = "");
    void SendRankStartInfo();
    void SendJoinRankFailedInfo();
    bool Join();
public slots:
    void SendValueChanged(int,QStringList);
    void SendFulled(QStringList);
    void recvs(QVariantMap);
private:
    rank_node *rkn;
    int nb;
    QString username;
    QString rkif;
    RequestProcesser* ntwkmgr;
    QString to;
};

#endif // RANKING_H
