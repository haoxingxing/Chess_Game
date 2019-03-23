#ifndef RANKING_H
#define RANKING_H
#include "event.h"
#include <QMap>
#include <QStringList>
#include <functional>
class rank_node {
public:
    rank_node(std::function<void(QStringList)> callback);
    void join(QString _username,QString _rank_id);
    void leave();
    bool isranking();
private:
    void fulled();
    bool is_ranking;
    static QMap<QString,QVector<QPair<QString,rank_node*>>> all_;
    static QMap<QString,QStringList> all_lists;
    std::function<void(QStringList)> callback;    
    QString rank_id;
    QString username;
};

























#endif // RANKING_H