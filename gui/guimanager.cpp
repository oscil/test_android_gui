#include "guimanager.h"

GUIManager* GUIManager::m_instance = NULL;

GUIManager::GUIManager(QObject *parent) :
    QObject(parent),
    m_itemsRateModel(NULL)
{
    m_engine = new QQmlApplicationEngine;
    connect(m_engine, SIGNAL(objectCreated(QObject*,QUrl)),
            this, SLOT(slotGUICreated(QObject*,QUrl)));
    m_rootContext =  m_engine->rootContext();
    m_rootContext->setContextProperty("GUIMgr",this);
    m_engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    m_rootObject = m_engine->rootObjects()[0];

    connect(m_rootObject, SIGNAL(signalRegistrationMain(QString, QString)),
            this, SIGNAL(signalRegistration(QString,QString)));
    connect(m_rootObject, SIGNAL(signalLogin(QString, QString)),
            this, SIGNAL(signalLogin(QString,QString)));
    connect(m_rootObject, SIGNAL(signalListIndexChanged(qint32, qint32)),
            this, SIGNAL(signalRateViewIndexChanged(qint32,qint32)));
    connect(m_rootObject, SIGNAL(signalRecommendListIndexChanged(qint32, qint32)),
            this, SIGNAL(signalRecommendViewIndexChanged(qint32,qint32)));
    connect(m_rootObject, SIGNAL(signalRateItem(qint32, qreal)),
            this, SIGNAL(signalRateItem(qint32, qreal)));
    connect(m_rootObject, SIGNAL(signalSkipItem(qint32)),
            this, SIGNAL(signalSkipItem(qint32)));
    connect(m_rootObject, SIGNAL(signalRateRecommendItem(qint32, qreal)),
            this, SIGNAL(signalRateRecommendItem(qint32, qreal)));
}

GUIManager *GUIManager::instance()
{
    if (m_instance == NULL)
        m_instance = new GUIManager;
    return m_instance;
}

void GUIManager::slotRegistrationStatusChanged(QString status, bool isOK)
{
    qDebug() << "status from server = " << status << " isOK = " << isOK;
    emit(signalRegistrationStatusChanged(status, isOK));
    statusString(status);
}

void GUIManager::slotLoginStatusChanged(QString status, bool isOK)
{
    emit(signalLoginStatusChanged(status, isOK));
    statusString(status);
}

void GUIManager::slotGUICreated(QObject *object, const QUrl &url)
{
    qDebug()<<"gui created gui manager";
    emit(signalGUICreated());
}

void GUIManager::onSignalRegistrationMain(QString login, QString password)
{
    qDebug() << "REGISTRATION! login = " << login << " password = " << password;
    startTimer(2000);
}

void GUIManager::timerEvent(QTimerEvent *event)
{
    statusString("reg ok");
    killTimer(event->timerId());
}

void GUIManager::slotCreateRatePage(QJsonObject* itemDescr)
{
    QObject *ratePage = m_componentRatePage->create(m_rootContext);
    QQuickItem *item = qobject_cast<QQuickItem*>(ratePage);
    item->setProperty("description", "Hey! description from c++!!!");
//    item->setParentItem((qobject_cast<QQuickWindow*>(m_rootObject))->contentItem());
//    m_engine->setObjectOwnership(item, QQmlEngine::JavaScriptOwnership);
    qDebug() << "root objects: " << m_engine->rootObjects();

    QQuickItem* stack = m_rootObject->findChild<QQuickItem*>("stack");
    qDebug() << "stack = " << stack;
    QQuickItem returnVal;
    QMetaObject::invokeMethod(stack, "push",
                              Q_RETURN_ARG(QQuickItem, returnVal),
                              Q_ARG(QQuickItem, *item));

}

void GUIManager::destroy()
{
    if (!m_instance) return;
    delete m_instance;
    m_instance = NULL;
}

GUIManager::~GUIManager()
{
    qDebug() << "destruct GUI";
}

