#ifndef COMMANDGETTOKEN_H
#define COMMANDGETTOKEN_H

#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"

class CheckerGetToken : public AbstractNetworkChecker{
public:
    CheckerGetToken(QNetworkReply* reply = NULL):
            AbstractNetworkChecker(reply)
        {
        }
    bool check(AbstractEvent *event);
};


class ProcessorGetToken : public AbstractNetworkProcessor{
    Q_OBJECT
public:
    void process(AbstractEvent* event);
};


class CommandGetToken : public AbstractNetworkCommand
{
     Q_OBJECT
public:
    CommandGetToken(QString* serverName,
                    QString* nonce,
                    QString* devKey,
                    QString* login,
                    QString* password);
    void execute();

protected:
    QString* m_serverName;
    QString* m_nonce;
    QString* m_devKey;
    QString* m_login;
    QString* m_password;
};

#endif // COMMANDGETTOKEN_H
