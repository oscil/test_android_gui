#ifndef COMMANDSKIPITEM_H
#define COMMANDSKIPITEM_H

#include <QJsonObject>
#include <QJsonDocument>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerSkipItem : public AbstractNetworkChecker{
public:
    CheckerSkipItem(quint64 id, QNetworkReply* rep = NULL):
        m_id(id),
        AbstractNetworkChecker(rep)
    {}
    bool check(AbstractEvent *event);
private:
    quint64 m_id;
};


class ProcessorSkipItem : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};


class CommandSkipItem : public AbstractNetworkCommand
{
    Q_OBJECT
public:
    explicit CommandSkipItem(QString* serverName,
                             QString* token,
                             qint64 id);
    void execute();

protected:
    QString* m_serverName;
    QString* m_token;
    qint64 m_id;
};

#endif // COMMANDSKIPITEM_H
