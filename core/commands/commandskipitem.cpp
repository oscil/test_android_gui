#include "commandskipitem.h"


CommandSkipItem::CommandSkipItem(QString* serverName,
                                 QString* token,
                                 qint64 id) :
    AbstractNetworkCommand(new CheckerSkipItem(id), new ProcessorSkipItem),
    m_serverName(serverName),
    m_token(token),
    m_id(id)
{
    this->setRepeatInterval(3000);
    this->setLifeTime(0);
    this->setRepeatType(REPEAT_INFINITELY);
    this->setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    setObjectName("CommandSkipItem");
}

void CommandSkipItem::execute()
{
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE SKIP ITEM";
    QNetworkRequest* request = new QNetworkRequest( *m_serverName  + "/processing/setItemRate");
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonObject jsonRequest;
    jsonRequest["token"]= *m_token;
    jsonRequest["itemId"] = QString::number(m_id);
    jsonRequest["rate"] = QString("-2");
    QJsonDocument jdoc(jsonRequest);
    qDebug() << "EXECUTE Skip Item json request = " << jdoc.toJson();
    emit signalRequest(request, jdoc.toJson());
    AbstractCommand::execute();
}


//---CHECKER

bool CheckerSkipItem::check(AbstractEvent *event){
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent->getNetworkReply();
        if (netReply == this->m_netReply){
            qDebug() << "CHECK COMMAND SKIP ITEM OK! id = " << m_id;
            return true;
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorSkipItem::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        qDebug() << "sent skip item process bytes = " << replyEvent->getNetworkReply()->bytesAvailable();
        emit signalReply(replyEvent->getNetworkReply());
    }
}

