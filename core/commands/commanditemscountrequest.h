#ifndef COMMANDITEMSCOUNTREQUEST_H
#define COMMANDITEMSCOUNTREQUEST_H

#include <QJsonObject>
#include <QJsonDocument>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerItemsCountRequest : public AbstractNetworkChecker{
public:
    CheckerItemsCountRequest(QNetworkReply* rep = NULL):
        AbstractNetworkChecker(rep)
    {
    }
    bool check(AbstractEvent *event);
};


class ProcessorItemsCountRequest : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};



class CommandItemsCountRequest : public AbstractNetworkCommand
{
    Q_OBJECT
public:
    CommandItemsCountRequest(QString* serverName,
                             QString* token,
                             quint32 count);
    void execute();

protected:
    QString* m_serverName;
    QString* m_token;
    quint32 m_countRequest;
};

#endif // COMMANDITEMSCOUNTREQUEST_H
