#ifndef RANKING_H
#define RANKING_H

#include <QObject>
#include "requestprocesser.h"
#include <QTimer>
#include "rqstprcs.h"

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

class ranking : public RQSTPRCS
{
    Q_OBJECT
public:
    explicit ranking(RequestProcesser* mnm = nullptr,int n=2,QString rank_info = "");

public slots:
    void SendValueChanged(int,QStringList);
    void SendFulled(QStringList);
    void recv(QVariantMap);
    void SendRankStartInfo();
    void SendJoinRankFailedInfo();
    bool Join();
    void SendJoined();
    void dscnktd();
private:
    rank_node *rkn;
    int nb;
    QString rkif;
};

#endif // RANKING_H
