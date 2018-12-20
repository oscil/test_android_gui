#ifndef COMMANDRATEITEM_H
#define COMMANDRATEITEM_H

#include <QJsonObject>
#include <QJsonDocument>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerRateItem : public AbstractNetworkChecker{
public:
    CheckerRateItem(quint64 id, QNetworkReply* rep = NULL):
        m_id(id),
        AbstractNetworkChecker(rep)
    {}
    bool check(AbstractEvent *event);
private:
    quint64 m_id;
};


class ProcessorRateItem : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};


class CommandRateItem : public AbstractNetworkCommand
{
    Q_OBJECT
public:
    explicit CommandRateItem(QString* serverName,
                             QString* token,
                             qint64 id,
                             double rate);
    void execute();

protected:
    QString* m_serverName;
    QString* m_token;
    qint64 m_id;
    double m_rate;
};

#endif // COMMANDRATEITEM_H
