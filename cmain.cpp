#include "cmain.h"

cmain::cmain(QObject *parent) :
    QObject(parent),
    m_settings("SokoSoft", "Recomendalka")
{
    //TODO сделать так, чтобы при большом входном потоке ничего не падало (например 1000 элементов подряд)
    //TODO сделать проверку на оставшиеся элементы при пропуске
    QString serverAddress = m_settings.value("serveraddress", "http://172.16.0.55:8080").toString();
    QString loginString = m_settings.value("login", "").toString();
    QString passwordString = m_settings.value("password", "").toString();

    m_gui = GUIManager::instance();
    if (!loginString.isEmpty())
        m_gui->loginString(loginString);
    if (!passwordString.isEmpty())
        m_gui->passwordString(passwordString);

    m_core = CCore::instance();
    m_core->setServerName("http://91.224.133.172:8080");
//    m_core->setServerName("http://127.0.0.1:8080");
    m_gui->setItemsRateModel(m_core->getItemsRateModel());
    m_gui->setItemsRecommendModel(m_core->getItemsRecommendModel());

    m_core->setDevKey("gbnd22op");

    connect(this, SIGNAL(signalSendNonceRequest()),
            m_core, SLOT(slotSendNonceRequest()));
    connect(m_gui, SIGNAL(signalRegistration(QString,QString)),
            m_core, SLOT(slotSendRegistrationRequest(QString,QString)));
    connect(m_gui, SIGNAL(signalLogin(QString,QString)),
            m_core, SLOT(slotSendLoginRequest(QString,QString)));
    connect(m_gui, SIGNAL(signalLogin(QString,QString)),
            this, SLOT(slotLogin(QString,QString)));
    connect(m_core, SIGNAL(signalRegistrationStatus(QString,bool)),
            m_gui, SLOT(slotRegistrationStatusChanged(QString,bool)));
    connect(m_core, SIGNAL(signalLoginStatus(QString,bool)),
            m_gui, SLOT(slotLoginStatusChanged(QString,bool)));
    connect(m_gui, SIGNAL(signalGUICreated()),
            this, SLOT(slotGUICreated()));
    connect(m_core, SIGNAL(signalReceiveNewRateDescription(QString)),
            m_gui, SIGNAL(signalReceiveNewRateDescription(QString)));
    connect(m_core, SIGNAL(signalReceiveNewRecommendDescription(QString)),
            m_gui, SIGNAL(signalReceiveNewRecommendDescription(QString)));
    connect(m_gui, SIGNAL(signalRateViewIndexChanged(qint32,qint32)),
            this, SLOT(slotRateViewIndexChanged(qint32, qint32)));
    connect(m_gui, SIGNAL(signalRecommendViewIndexChanged(qint32,qint32)),
            this, SLOT(slotRecommendViewIndexChanged(qint32, qint32)));
    connect(this, SIGNAL(signalRequestItemsCount(quint32)),
            m_core, SLOT(slotSendRequestItemsCount(quint32)));
    connect(this, SIGNAL(signalRequestRecommendItemsCount(quint32)),
            m_core, SLOT(slotSendRequestRecommendItemsCount(quint32)));
    connect(m_gui, SIGNAL(signalRateItem(qint32,qreal)),
            m_core, SLOT(slotSendRateItem(qint32,qreal)));
    connect(m_gui, SIGNAL(signalSkipItem(qint32)),
            m_core, SLOT(slotSendSkipItem(qint32)));
    connect(m_gui, SIGNAL(signalRateRecommendItem(qint32,qreal)),
            m_core, SLOT(slotSendRateRecommendItem(qint32,qreal)));

    emit signalSendNonceRequest();
}

void cmain::slotRateViewIndexChanged(qint32 currentIndex, qint32 count)
{
    //TODO лучше сделать проверку на оставшиеся элементы
//    static qint32 lastRequestCount = 0;
    qDebug() << "SERVER rateView index changed cur = " << currentIndex << " count = " << count;
//    if ( (count == currentIndex+1) ||
//            ( (count - currentIndex) <= 5
//         && lastRequestCount < count ) ){
//        lastRequestCount = count;
//        qDebug() << "Send items request";
//        emit signalRequestItemsCount(5);
//    }
    if (count - currentIndex < 4){
        emit signalRequestItemsCount(15);
    }
}

void cmain::slotRecommendViewIndexChanged(qint32 currentIndex, qint32 count)
{
//    static qint32 lastRequestCount = 0;
//    qDebug() << "SERVER recomendView index changed cur = " << currentIndex << " count = " << count << " lastRequestCount = " << lastRequestCount;
//    if ( (count == currentIndex+1) ||
//            ( (count - currentIndex) <= 3
//         && lastRequestCount < count ) ){
//        lastRequestCount = count;
//        qDebug() << "Send recomend items request";
//        emit signalRequestRecommendItemsCount(3);
//    }

    qDebug() << "SERVER Recommend View index changed cur = " << currentIndex << " count = " << count;
    if (count - currentIndex < 4){
        emit signalRequestRecommendItemsCount(15);
    }
}

void cmain::slotLogin(QString login, QString password)
{
    m_settings.setValue("login", login);
    m_settings.setValue("password", password);
}

void cmain::slotGUICreated()
{

}

cmain::~cmain()
{
    GUIManager::destroy();
    CCore::destroy();

}

