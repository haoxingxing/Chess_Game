#ifndef RANKING_H
#define RANKING_H
#include "event.h"
#include <QMap>
#include <QPair>
#include <QObject>
#include <QStringList>
#include <functional>
/*
 * Notice:
 *     The rank_node will trigger fulled() before the LAST join() finished
 */
class rank_node : QObject {
    Q_OBJECT
public:
    rank_node(std::function<void(QStringList)> callback);
    bool join(QString _username,QString _rank_id);
    void leave();
    bool isranking();
    bool isfulled(QString _rank_id);
    void triggerFulled(QString _rank_id);
    static QString get_a_rank_id(int number);
private:
    struct datas{
        void insert(QString u,rank_node* p) {
            members.append(QPair<QString,rank_node*>(u,p));
        }
        datas(int n):numbers(n) {
        }
        int numbers;
    public:
        QVector<QPair<QString,rank_node*>> members;
    };
    void fulled();
    bool is_ranking;
    static QMap<QString,datas> data;
    bool check_is_fulled(QString _rank_id);
    std::function<void(QStringList)> callback;    
    QString rank_id;
    QString username;
};

























#endif // RANKING_H
