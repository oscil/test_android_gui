#ifndef COMMANDGETNONCE_H
#define COMMANDGETNONCE_H

#include <QJsonObject>
#include <QJsonDocument>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerGetNonce : public AbstractNetworkChecker{
public:
    CheckerGetNonce(QNetworkReply* reply = NULL);
    bool check(AbstractEvent *event);
};


class ProcessorGetNonce : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};


class CommandGetNonce : public AbstractNetworkCommand
{
     Q_OBJECT
public:
    CommandGetNonce(QString* serverName);
    void execute();

private:
    QString* m_serverName;
};

#endif // COMMANDGETNONCE_H
