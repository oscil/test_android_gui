#ifndef CCORE_H
#define CCORE_H

#include <QObject>
#include <QSet>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "commands/commandmgr.h"
#include "core/serverconnection.h"
#include "core/commands/events.h"
#include "core/commands/commandgetnonce.h"
#include "core/commands/commandgettoken.h"
#include "core/commands/commanditemdescriptionrequest.h"
#include "core/commands/commanditemscountrequest.h"
#include "core/commands/commandrecommenditemscountrequest.h"
#include "core/commands/commandlogin.h"
#include "core/commands/commandregister.h"
#include "core/commands/commandrateitem.h"
#include "core/commands/commandskipitem.h"
#include "common/itemsmodel.h"

class CCore : public QObject
{
    Q_OBJECT
public:
    static CCore* instance();
    static void destroy();
    inline void setServerName(QString name) {m_serverName = name;}
    inline void setDevKey(QString key) {m_devKey = key;}
    inline ItemsModel* getItemsRateModel() {return m_itemsRateModel;}
    inline ItemsModel* getItemsRecommendModel() {return m_itemsRecommendModel;}
    ~CCore();

signals:
    void signalOccuredEvent(AbstractEvent*);
    void signalDeleteCommand(AbstractCommand*);

    void signalRegistrationStatus(QString status, bool isOK);
    void signalLoginStatus(QString status, bool isOK);
    void signalReceiveNewRateDescription(QString);
    void signalReceiveNewRecommendDescription(QString);

public slots:
    //Reply processors
    void slotRecvNetworkReply(QNetworkReply*);
    void slotFinishedNetworkReply();
    void slotRecvNonceReply(QNetworkReply*);
    void slotRecvRegisterReply(QNetworkReply*);
    void slotRecvGetTokenReply(QNetworkReply*);
    void slotRecvLoginReply(QNetworkReply*);
    void slotRecvItemsCountReply(QNetworkReply*);
    void slotRecvRecommendItemsCountReply(QNetworkReply*);
    void slotRecvItemInfoReply(QNetworkReply*);
    void slotRecvRecommendItemInfoReply(QNetworkReply*);
    void slotRecvRateItemReply(QNetworkReply*);
    void slotRecvSkipItemReply(QNetworkReply*);

    //Request slots
    void slotSendNonceRequest();
    void slotSendRegistrationRequest(QString login,QString password);
    void slotSendLoginRequest(QString login,QString password);
    void slotSendRequestItemsCount(quint32 count);
    void slotSendRequestRecommendItemsCount(quint32 count);
    void slotSendRateItem(qint32,qreal);
    void slotSendRateRecommendItem(qint32,qreal);
    void slotSendSkipItem(qint32);

    //ERRORS
    void slotCmdError(QString);

private:
    CommandMgr* m_commandMgr;
    serverConnection* m_connectionMgr;

    static CCore* m_instance;
    CCore(QObject *parent = 0);

    QString m_serverName;
    QString m_devKey;

    QString m_nonce;
    QString m_login;
    QString m_password;
    QString m_token;

    QJsonObject getJSONObjectFromNetworkReply(QNetworkReply*);
    QSet<qint64> m_itemsIDFresh;
    QSet<qint64> m_itemsIDTotal;

    QSet<qint64> m_recommendItemsIDFresh;
    QSet<qint64> m_recommendItemsIDTotal;

    ItemsModel* m_itemsRateModel;
    ItemsModel* m_itemsRecommendModel;

    void addNetworkCommand(AbstractNetworkCommand*);
    void recvNewDescription(ItemsModel* model, QJsonObject* descr);
};

#endif // CCORE_H
