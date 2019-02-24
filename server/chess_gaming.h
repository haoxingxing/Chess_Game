#ifndef CHESS_GAMING_H
#define CHESS_GAMING_H

#include <QObject>
#include <QVector>
#include "requestprocesser.h"
class chess_gaming : public QObject
{
    Q_OBJECT
public:
    explicit chess_gaming(int pan_w,int pan_h,QObject *parent = nullptr,RequestProcesser* mnm = nullptr);
    void SendStartInfo();

public slots:
    void recv_process(QVariantMap);
private:
    struct zi{
        bool isCovered;
        QString who;
    };
    //[Height][Widght]
    QVector<QVector<zi>> pan;
    RequestProcesser* ntwkmgr;
    int pan_w,pan_h;
};

#endif // CHESS_GAMING_H
