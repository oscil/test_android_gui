#include "commanditemscountrequest.h"

CommandItemsCountRequest::CommandItemsCountRequest(QString* serverName,
                                                   QString* token,
                                                   quint32 count
                                                   ):
    AbstractNetworkCommand(new CheckerItemsCountRequest, new ProcessorItemsCountRequest),
    m_serverName(serverName),
    m_token(token),
    m_countRequest(count)
{
    this->setRepeatInterval(10000);
    this->setLifeTime(0);
    this->setRepeatType(REPEAT_INFINITELY);
    this->setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    setObjectName("CommandItemsCountRequest");
}

void CommandItemsCountRequest::execute()
{
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE ITEM COUNT REQUEST";
    QNetworkRequest* request = new QNetworkRequest(*m_serverName + "/processing/getItemsToRate");
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonObject jsonRequest;
    jsonRequest["token"]= *m_token;
    jsonRequest["count"] = QString::number(m_countRequest);
    QJsonDocument jdoc(jsonRequest);
    qDebug() << "EXECUTE ItemsCountRequest json request = " << jdoc.toJson();
    emit signalRequest(request, jdoc.toJson());
    AbstractCommand::execute();
}



//---CHECKER

bool CheckerItemsCountRequest::check(AbstractEvent *event){
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent->getNetworkReply();
        if (netReply->request().url().toString().contains("/processing/getItemsToRate") ||
            netReply == this->m_netReply){
            qDebug() << "CHECK COMMAND ITEM COUNT REQUEST OK!";
            return true;
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorItemsCountRequest::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        emit signalReply(replyEvent->getNetworkReply());
    }
}

