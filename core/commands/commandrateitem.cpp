#include "commandrateitem.h"

CommandRateItem::CommandRateItem(QString* serverName,
                                 QString* token,
                                 qint64 id,
                                 double rate) :
    AbstractNetworkCommand(new CheckerRateItem(id), new ProcessorRateItem),
    m_serverName(serverName),
    m_token(token),
    m_id(id),
    m_rate(rate)
{
    this->setRepeatInterval(3000);
    this->setLifeTime(0);
    this->setRepeatType(REPEAT_INFINITELY);
    this->setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    setObjectName("CommandRateItem");
}

void CommandRateItem::execute()
{
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE RATE ITEM";
    QNetworkRequest* request = new QNetworkRequest( *m_serverName  + "/processing/setItemRate");
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonObject jsonRequest;
    jsonRequest["token"]= *m_token;
    jsonRequest["itemId"] = QString::number(m_id);
    jsonRequest["rate"] = QString::number(m_rate);
    QJsonDocument jdoc(jsonRequest);
    qDebug() << "EXECUTE Rate Item json request = " << jdoc.toJson();
    emit signalRequest(request, jdoc.toJson());
    AbstractCommand::execute();
}


//---CHECKER

bool CheckerRateItem::check(AbstractEvent *event){
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent->getNetworkReply();
        if (netReply == this->m_netReply){
            qDebug() << "CHECK COMMAND RATE ITEM OK! id = " << m_id;
            return true;
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorRateItem::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        qDebug() << "sent rate item process bytes = " << replyEvent->getNetworkReply()->bytesAvailable();
        emit signalReply(replyEvent->getNetworkReply());
    }
}
