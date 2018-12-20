#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QQuickItem>
#include <QThread>
#include <QTimerEvent>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>

#include "common/commondefines.h"
#include "common/itemsmodel.h"

class GUIManager : public QObject
{
    Q_OBJECT
public:
    static GUIManager* instance();
    static void destroy();
    inline void setItemsRateModel(ItemsModel* model) {
        m_itemsRateModel = model;
        m_rootContext->setContextProperty("CPPItemsRateModel", m_itemsRateModel);
    }
    inline void setItemsRecommendModel(ItemsModel* model) {
        m_itemsRecommendModel = model;
        m_rootContext->setContextProperty("CPPItemsRecommendModel", m_itemsRecommendModel);
    }
    AUTO_PROPERTY(QString, statusString)
    AUTO_PROPERTY(bool, isWaiting)
    AUTO_PROPERTY(QString, loginString)
    AUTO_PROPERTY(QString, passwordString)


    ~GUIManager();

signals:
    void signalRegistration(QString login, QString password);
    void signalRegistrationStatusChanged(QString status, bool isOK);
    void signalLoginStatusChanged(QString status, bool isOK);
    void signalLogin(QString, QString);
    void signalGUICreated();
    void signalReceiveNewRateDescription(QString description);
    void signalReceiveNewRecommendDescription(QString description);
    void signalRateViewIndexChanged(qint32 currentIndex, qint32 indexCount);
    void signalRecommendViewIndexChanged(qint32 currentIndex, qint32 indexCount);
    void signalRateItem(qint32 index, qreal rate);
    void signalSkipItem(qint32 index);
    void signalRateRecommendItem(qint32 index, qreal rate);


public slots:
    void onSignalRegistrationMain(QString login, QString password);

    void slotRegistrationStatusChanged(QString status, bool isOK);
    void slotLoginStatusChanged(QString status, bool isOK);
    void slotGUICreated(QObject * object, const QUrl & url);
    void slotCreateRatePage(QJsonObject* itemDescr);

protected:
    void timerEvent(QTimerEvent *event);

private:
    GUIManager(QObject *parent = 0);
    static GUIManager* m_instance;
    QQmlApplicationEngine* m_engine;
    QObject* m_rootObject;
    QQmlContext* m_rootContext;

    QQmlComponent* m_componentRatePage;
    ItemsModel* m_itemsRateModel;
    ItemsModel* m_itemsRecommendModel;
};

#endif // GUIMANAGER_H
