#ifndef CMAIN_H
#define CMAIN_H

#include <QObject>
#include <QThread>
#include <QSettings>

#include "gui/guimanager.h"
#include "core/serverconnection.h"
#include "core/ccore.h"

class cmain : public QObject
{
    Q_OBJECT
public:
    explicit cmain(QObject *parent = 0);
    ~cmain();

private:
    GUIManager* m_gui;
    CCore* m_core;
    QString m_devKey;
    QSettings m_settings;

public slots:
    void slotRateViewIndexChanged(qint32, qint32);
    void slotRecommendViewIndexChanged(qint32, qint32);
    void slotGUICreated();
    void slotLogin(QString, QString);

signals:
    void signalRequestItemsCount(quint32 count);
    void signalRequestRecommendItemsCount(quint32 count);
    void signalSendNonceRequest();
};

#endif // CMAIN_H
