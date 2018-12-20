#include "commandlogin.h"

CommandLogin::CommandLogin(QString* serverName,
                           QString* nonce,
                           QString* devKey,
                           QString* login,
                           QString* password):
    CommandGetToken(serverName, nonce, devKey, login, password)
{
    setObjectName("CommandGetToken");
}

