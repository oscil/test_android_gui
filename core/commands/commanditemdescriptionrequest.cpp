#include "commanditemdescriptionrequest.h"

CommandItemDescriptionRequest::CommandItemDescriptionRequest(QString* serverName,
                                                             QString* token,
                                                             qint64 id):
    AbstractNetworkCommand(new CheckerItemDescriptionRequest(id), new ProcessorItemDescriptionRequest),
    m_serverName(serverName),
    m_token(token),
    m_id(id)
{
    this->setRepeatInterval(10000);
    this->setLifeTime(0);
    this->setRepeatType(REPEAT_INFINITELY);
    this->setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    setObjectName("CommandItemDescriptionRequest");
}

void CommandItemDescriptionRequest::execute()
{
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE ITEM DESCRIPTION REQUEST";
    QNetworkRequest* request = new QNetworkRequest( *m_serverName  + "/content/getItemInfo");
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonObject jsonRequest;
    jsonRequest["token"]= *m_token;
    jsonRequest["itemId"] = QString::number(m_id);
    QJsonDocument jdoc(jsonRequest);
    qDebug() << "EXECUTE Item Description Request json request = " << jdoc.toJson();
    emit signalRequest(request, jdoc.toJson());
    AbstractCommand::execute();
}



//---CHECKER

bool CheckerItemDescriptionRequest::check(AbstractEvent *event){
    static QByteArray reply;
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent?replyEvent->getNetworkReply():NULL;
        if (netReply && netReply->request().url().toString().contains("/content/getItemInfo")){
//            reply = netReply->peek(netReply->bytesAvailable());//TODO ВЕРНИ СЮДА ПИИИИИК!!!!
//            QJsonDocument jdoc = QJsonDocument::fromJson(reply);
//            QJsonObject jobj = jdoc.object();
//            qint64 id = jobj["itemId"].toString().toLongLong();
            if (netReply == this->m_netReply){
                qDebug() << "CHECK COMMAND ITEM DESCRIPTION REQUEST OK! id = " << m_id;
                return true;
            }
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorItemDescriptionRequest::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        qDebug() << "sent item description process bytes = " << replyEvent->getNetworkReply()->bytesAvailable();
        emit signalReply(replyEvent->getNetworkReply());
    }
}
