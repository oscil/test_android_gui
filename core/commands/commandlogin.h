#ifndef COMMANDLOGIN_H
#define COMMANDLOGIN_H

#include "core/commands/abstractnetworkcommand.h"
#include "core/commands/events.h"
#include "core/commands/commandgettoken.h"


class CommandLogin : public CommandGetToken
{
    Q_OBJECT
public:
    CommandLogin(QString* serverName,
                 QString* nonce,
                 QString* devKey,
                 QString* login,
                 QString* password);
};

#endif // COMMANDLOGIN_H
