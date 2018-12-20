#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QTimer>
#include <QList>
#include <QThread>

#include "core/commands/abstractnetworkcommand.h"

class serverConnection : public QThread
{
    Q_OBJECT
public:
    static serverConnection* instance();
    ~serverConnection();

signals:
    void signalRecvNetworkReply(QNetworkReply*);

public slots:
    void slotPost(QNetworkRequest* request, QByteArray data);
    void slotPostCommand(QNetworkRequest* request, QByteArray data);

private:
    QNetworkAccessManager* m_netMgr;
    static serverConnection* m_singletonServer;
    serverConnection(QObject *parent = 0);
};

#endif // SERVERCONNECTION_H
