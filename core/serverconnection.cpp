#include "serverconnection.h"

serverConnection* serverConnection::m_singletonServer = NULL;

serverConnection::serverConnection(QObject *parent) : QThread(parent)
{
    m_netMgr = new QNetworkAccessManager(this);
    connect(m_netMgr, SIGNAL(finished(QNetworkReply*)),
            this, SIGNAL(signalRecvNetworkReply(QNetworkReply*)));
    start();
//    m_netMgr->moveToThread(this);
}

serverConnection* serverConnection::instance()
{
    if(!m_singletonServer)
        m_singletonServer = new serverConnection;
    return m_singletonServer;
}

void serverConnection::slotPost(QNetworkRequest* request,QByteArray data)
{
    if (data.isEmpty())
        m_netMgr->get(*request);
    else
        m_netMgr->post(*request, data);
    delete request;
}

void serverConnection::slotPostCommand(QNetworkRequest* request, QByteArray data)
{
    AbstractNetworkCommand* cmd = dynamic_cast<AbstractNetworkCommand*>(sender());
    if (cmd){
        QNetworkReply* reply;
        if (data.isEmpty())
            reply = m_netMgr->get(*request);
        else
            reply = m_netMgr->post(*request, data);
        delete request;
        cmd->setReply(reply);
    }
}

serverConnection::~serverConnection()
{
    quit();
}

