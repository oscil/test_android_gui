#include "commandgettoken.h"

CommandGetToken::CommandGetToken(QString* serverName,
                                 QString* nonce,
                                 QString* devKey,
                                 QString* login,
                                 QString* password
                                 ):
    AbstractNetworkCommand(new CheckerGetToken, new ProcessorGetToken),
    m_serverName(serverName),
    m_nonce(nonce),
    m_devKey(devKey),
    m_login(login),
    m_password(password)
{
    this->setRepeatInterval(2000);
    this->setLifeTime(0);
    this->setRepeatType(REPEAT_INFINITELY);
    this->setEventType(AbstractEvent::EVENT_NETWORK_REPLY);
    setObjectName("CommandGetToken");
}

void CommandGetToken::execute()
{
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE GET TOKEN";
    QCryptographicHash chash(QCryptographicHash::Md5);
    QByteArray arr;
    arr.append(*m_nonce).append(*m_login).append(*m_password).append(*m_devKey);
    chash.addData(arr.data(), arr.size());
    QNetworkRequest* request = new QNetworkRequest(*m_serverName + "/auth/getToken");
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonObject jsonRequest;
    jsonRequest["name"]= *m_login;
    jsonRequest["hash"] = QString(chash.result().toHex());
    QJsonDocument jdoc(jsonRequest);
    qDebug() << "EXECUTE get token json request = " << jdoc.toJson();
    emit signalRequest(request, jdoc.toJson());
    AbstractCommand::execute();
}



//---CHECKER


bool CheckerGetToken::check(AbstractEvent *event){
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent->getNetworkReply();
        if (netReply != m_netReply)
            return false;
        if (netReply->request().url().toString().contains("/auth/getToken")){
            qDebug() << "CHECK COMMAND GET TOKEN OK!";
            return true;
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorGetToken::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        emit signalReply(replyEvent->getNetworkReply());
    }
}

