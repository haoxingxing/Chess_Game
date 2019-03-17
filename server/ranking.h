#ifndef RANKING_H
#define RANKING_H

#include <QObject>
#include "mainnetworkmanger.h"
#include <QTimer>
#include "event.h"

/* Request Respone
 * recv:
 *      101: exit {}
 *      102: join {}
 * send:
 *      100: join failed {}
 *      101: rank fulled {+players(QStringList)}
 *      102: rank started {}
 *      103: rank list changed {+nnb(int) +list(QStringList)}
 *      104: joined {}
*/
class ranking;
class rank_node : public QObject
{
    Q_OBJECT
public:
    struct Rank_Node {
        int now;
        QStringList user_list;
        int fulledSiganlSended = 0;
    };
    explicit rank_node(ranking *parent,int n,QString rank_info);
    bool join(QString username);
    void check_full();
    void del(QString username);
private slots:
    void check_number_changed();
private:
    static QMap<QString,Rank_Node> rn;
    int nb;
    QString rkif;
    bool isranking = false;
    QTimer tm_ch_sg;
    int now_in_this_class;
    ranking* pt;
};

class ranking : public Event
{
    Q_OBJECT
public:
    explicit ranking(MainNetworkManger*rp,QString evid,EventManger* e);
signals:
    void fulled(QStringList list);
public slots:
    void SendValueChanged(int,QStringList);
    void SendFulled(QStringList);
    void recv(const int&,const QVariantMap&);
    bool Join();
    void dscnktd();
private:
    rank_node *rkn = nullptr;
    int nb;
    QString rkif;
};

#endif // RANKING_H
