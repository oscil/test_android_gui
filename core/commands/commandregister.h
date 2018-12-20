#ifndef COMMANDREGISTER_H
#define COMMANDREGISTER_H

#include <QJsonDocument>
#include <QJsonObject>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerRegister : public AbstractNetworkChecker{
public:
    CheckerRegister(QNetworkReply* rep = NULL):
        AbstractNetworkChecker(rep)
    {
    }
    bool check(AbstractEvent *event);
};


class ProcessorRegister : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};

class CommandRegister : public AbstractNetworkCommand
{
    Q_OBJECT
public:
    CommandRegister(QString* serverName,
                    QString* nonce,
                    QString* devKey,
                    QString* login,
                    QString* password);
    void execute();

private:
    QString* m_serverName;
    QString* m_nonce;
    QString* m_devKey;
    QString* m_login;
    QString* m_password;
};

#endif // COMMANDREGISTER_H
