#ifndef REQUESTPROCESSER_H
#define REQUESTPROCESSER_H

#include <QObject>
#include <QTcpSocket>
class RequestProcesser : public QObject
{
    Q_OBJECT
public:
    explicit RequestProcesser(QObject *parent = nullptr,QTcpSocket* so = nullptr);
    void send(QString str);
    //How To :
    void ToLogin();
    void SendLoginErrorMessage();
    void SendLoginSuccessMessage();
    enum LoginType{
        LOGIN,
        REGISTER
    };
    void Logout();
    void LoginProcess(QVariantMap map,LoginType tp);
signals:
    void Message(QVariantMap);
    void dscnktd();
public slots:
    void readyRead();
private:
    QTcpSocket* socket;

    QString username;
    bool isLogin = false;
};

#endif // REQUESTPROCESSER_H