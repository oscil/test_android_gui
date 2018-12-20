#include "commandrecommenditemscountrequest.h"

CommandRecommendItemsCountRequest::CommandRecommendItemsCountRequest(QString* serverName,
                                                   QString* token,
                                                   quint32 count
                                                   ):
    AbstractNetworkCommand(new CheckerRecommendItemsCountRequest,
                           new ProcessorRecommendItemsCountRequest),
    m_serverName(serverName),
    m_token(token),
    m_countRequest(count)
{
    this->setRepeatInterval(2000);
    this->setLifeTime(0);
    this->setRepeatType(REPEAT_INFINITELY);
    this->setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    setObjectName("CommandRecommendItemsCountRequest");
}

void CommandRecommendItemsCountRequest::execute()
{
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE RECOMEND ITEMS COUNT REQUEST";
    QNetworkRequest* request = new QNetworkRequest(*m_serverName + "/processing/getListWithRecom");
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonObject jsonRequest;
    jsonRequest["token"]= *m_token;
    jsonRequest["count"] = QString::number(m_countRequest);
    QJsonDocument jdoc(jsonRequest);
    qDebug() << "EXECUTE RecommendItemsCountRequest json request = " << jdoc.toJson();
    emit signalRequest(request, jdoc.toJson());
    AbstractCommand::execute();
}



//---CHECKER

bool CheckerRecommendItemsCountRequest::check(AbstractEvent *event){
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent->getNetworkReply();
        if (netReply->request().url().toString().contains("/processing/getListWithRecom") ||
            netReply == this->m_netReply){
            qDebug() << "CHECK COMMAND RECOMEND ITEM COUNT REQUEST OK!";
            return true;
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorRecommendItemsCountRequest::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        emit signalReply(replyEvent->getNetworkReply());
    }
}



