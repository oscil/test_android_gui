#include "commandgetnonce.h"

CommandGetNonce::CommandGetNonce(QString* serverName):
    AbstractNetworkCommand(new CheckerGetNonce, new ProcessorGetNonce),
    m_serverName(serverName)
{
    this->setRepeatInterval(3000);
    this->setLifeTime(0);
    this->setRepeatType(REPEAT_INFINITELY);
    this->setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    setObjectName("CommandGetNonce");
}

void CommandGetNonce::execute()
{
    if (m_reply)
        qDebug() << "is running getNonce = " << m_reply->isRunning();
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE GET NONCE";
    QNetworkRequest* request = new QNetworkRequest(*m_serverName + "/auth/getNonce");
    emit signalRequest(request, QByteArray());
    AbstractCommand::execute();
}

//---CHECKER

CheckerGetNonce::CheckerGetNonce(QNetworkReply* reply):
    AbstractNetworkChecker(reply)
{
}

bool CheckerGetNonce::check(AbstractEvent *event){
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent->getNetworkReply();
        if (netReply != m_netReply)
            return false;
        QByteArray reply = netReply->peek(netReply->bytesAvailable());
        QJsonDocument jdoc = QJsonDocument::fromJson(reply);
        QJsonObject jobj = jdoc.object();
        QString status = jobj["status"].toString();
        if (status == "ok"){
            qDebug() << "CHECK COMMAND GET NONCE OK!";
            return true;
        }
        else{
            qDebug() << "ERROR in nonce reply = " << reply;
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorGetNonce::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        emit signalReply(replyEvent->getNetworkReply());
    }
}

