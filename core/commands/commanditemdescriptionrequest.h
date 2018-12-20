#ifndef COMMANDITEMDESCRIPTIONREQUEST_H
#define COMMANDITEMDESCRIPTIONREQUEST_H

#include <QJsonDocument>
#include <QJsonObject>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerItemDescriptionRequest : public AbstractNetworkChecker{
public:
    CheckerItemDescriptionRequest(quint64 id, QNetworkReply* rep = NULL):
        m_id(id),
        AbstractNetworkChecker(rep)
    {}
    bool check(AbstractEvent *event);
private:
    quint64 m_id;
};


class ProcessorItemDescriptionRequest : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};


class CommandItemDescriptionRequest : public AbstractNetworkCommand
{
    Q_OBJECT
public:
    CommandItemDescriptionRequest(QString* serverName,
                                  QString* token,
                                  qint64 id);
    void execute();

protected:
    QString* m_serverName;
    QString* m_token;
    qint64 m_id;
};

#endif // COMMANDITEMDESCRIPTIONREQUEST_H
