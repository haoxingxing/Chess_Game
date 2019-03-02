#ifndef RQSTPRCS_H
#define RQSTPRCS_H

#include <QObject>
#include "requestprocesser.h"

class RQSTPRCS : public QObject
{
    Q_OBJECT
public:
    explicit RQSTPRCS(QString name,RequestProcesser *parent = nullptr);
    virtual void recv(QVariantMap) = 0;
    virtual void dscnktd() = 0;
    RequestProcesser* ntwkmgr;
signals:

public slots:
    void recv_t(QVariantMap);
private:
    QString class_name;
};

#endif // RQSTPRCS_H
