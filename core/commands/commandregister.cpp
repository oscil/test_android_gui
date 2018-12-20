#include "commandregister.h"

CommandRegister::CommandRegister(QString* serverName,
                                 QString* nonce,
                                 QString* devKey,
                                 QString* login,
                                 QString* password):
    AbstractNetworkCommand(new CheckerRegister, new ProcessorRegister),
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
    setObjectName("CommandRegister");
}

void CommandRegister::execute()
{
    if (m_reply && m_reply->isRunning()) return;
    qDebug() << "COMMAND EXECUTE REGISTER";
    QCryptographicHash chash(QCryptographicHash::Md5);
    QByteArray arr;
    arr.append(*m_nonce).append(*m_login).append(*m_password).append(*m_devKey);
    chash.addData(arr.data(), arr.size());
    QNetworkRequest* request = new QNetworkRequest(*m_serverName + "/auth/registerUser");
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonObject jsonRequest;
    jsonRequest["name"]= *m_login;
    jsonRequest["password"] = *m_password;
    jsonRequest["hash"] = QString(chash.result().toHex());
    QJsonDocument jdoc(jsonRequest);
    qDebug() << "EXECUTE Register json request = " << jdoc.toJson();
    emit signalRequest(request, jdoc.toJson());
    AbstractCommand::execute();
}


//---CHECKER

bool CheckerRegister::check(AbstractEvent *event){
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        QNetworkReply* netReply = replyEvent->getNetworkReply();
        if (netReply->request().url().toString().contains("/auth/registerUser")){
            qDebug() << "CHECK COMMAND REGISTER OK!";
            return true;
        }
    }
    return false;
}

//-----PROCESSOR
void ProcessorRegister::process(AbstractEvent* event)
{
    if (event->eventType() == AbstractEvent::EVENT_NETWORK_REPLY){
        NetworkReplyEvent* replyEvent = static_cast<NetworkReplyEvent*>(event);
        emit signalReply(replyEvent->getNetworkReply());
    }
}


