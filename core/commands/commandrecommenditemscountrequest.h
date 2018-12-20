#ifndef COMMANDRECOMMENDITEMSCOUNTREQUEST_H
#define COMMANDRECOMMENDITEMSCOUNTREQUEST_H

#include <QJsonObject>
#include <QJsonDocument>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerRecommendItemsCountRequest : public AbstractNetworkChecker{
public:
    CheckerRecommendItemsCountRequest(QNetworkReply* rep = NULL):
        AbstractNetworkChecker(rep)
    {
    }
    bool check(AbstractEvent *event);
};


class ProcessorRecommendItemsCountRequest : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};



class CommandRecommendItemsCountRequest : public AbstractNetworkCommand
{
    Q_OBJECT
public:
    CommandRecommendItemsCountRequest(QString* serverName,
                                      QString* token,
                                      quint32 count);
    void execute();

protected:
    QString* m_serverName;
    QString* m_token;
    quint32 m_countRequest;
};


#endif // COMMANDRECOMMENDITEMSCOUNTREQUEST_H
