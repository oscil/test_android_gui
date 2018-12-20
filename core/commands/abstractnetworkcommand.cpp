#include "abstractnetworkcommand.h"

AbstractNetworkChecker::AbstractNetworkChecker(QNetworkReply* reply):
    m_netReply(reply)
{
}

AbstractNetworkCommand::AbstractNetworkCommand(AbstractNetworkChecker* checker,
                                               AbstractNetworkProcessor* processor):
    AbstractCommand(checker, processor),
    m_reply(NULL),
    m_netChecker(checker)
{
    setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    connect(processor, SIGNAL(signalReply(QNetworkReply*)),
            this, SIGNAL(signalReply(QNetworkReply*)));
}

AbstractNetworkCommand::~AbstractNetworkCommand()
{
    disconnect(this);
    m_reply->deleteLater();
}

