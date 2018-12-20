#include "ccore.h"

CCore* CCore::m_instance = NULL;

CCore* CCore::instance()
{
    if(m_instance == NULL)
        m_instance = new CCore;
    return m_instance;
}

CCore::CCore(QObject *parent) : QObject(parent)
{
    m_itemsRateModel = new ItemsModel(this);
    m_itemsRecommendModel = new ItemsModel(this);

    m_commandMgr = CommandMgr::instance();
    m_connectionMgr = serverConnection::instance();

    connect(m_connectionMgr, SIGNAL(signalRecvNetworkReply(QNetworkReply*)),
            this, SLOT(slotRecvNetworkReply(QNetworkReply*)));
    connect(this, SIGNAL(signalOccuredEvent(AbstractEvent*)),
            m_commandMgr, SLOT(slotOccuredEvent(AbstractEvent*)));
    connect(this, SIGNAL(signalDeleteCommand(AbstractCommand*)),
            m_commandMgr, SLOT(slotDeleteCommand(AbstractCommand*)));
}

QJsonObject CCore::getJSONObjectFromNetworkReply(QNetworkReply* _reply)
{
    QJsonObject jobj;
    static QByteArray reply;
    if (_reply &&
            _reply->header(QNetworkRequest::ContentTypeHeader).isValid() &&
            _reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json") ){
        reply = _reply->readAll();
        if(!reply.isEmpty()){
            QJsonDocument jdoc = QJsonDocument::fromJson(reply);
            jobj = jdoc.object();
        }
    }
    return jobj;
}

void CCore::slotRecvNetworkReply(QNetworkReply *_reply)
{
    qDebug() << "recv net reply tid = " << QThread::currentThreadId();
    if (!_reply->isFinished()){
        _reply->moveToThread(this->thread());
        connect(_reply, SIGNAL(finished()),
                this, SLOT(slotFinishedNetworkReply()));
    }
    else {
        _reply->moveToThread(this->thread());
        NetworkReplyEvent* netEvent = new NetworkReplyEvent(_reply);
        emit signalOccuredEvent(netEvent);
    }
}

void CCore::slotFinishedNetworkReply()
{
    QNetworkReply* _reply = dynamic_cast<QNetworkReply*>(sender());
    qDebug() << "REPLY FINISHED reply = " << _reply;
    if (_reply){
        NetworkReplyEvent* netEvent = new NetworkReplyEvent(_reply);
        emit signalOccuredEvent(netEvent);
    }
}

void CCore::addNetworkCommand(AbstractNetworkCommand* cmd)
{
    connect(cmd, SIGNAL(signalRequest(QNetworkRequest*,QByteArray)),
            m_connectionMgr, SLOT(slotPostCommand(QNetworkRequest*,QByteArray)));
    //TODO подумать про удаление команды с ошибкой
    connect(cmd, SIGNAL(signalCommandError(QString)),
            this, SLOT(slotCmdError(QString)));
    m_commandMgr->pushCommand(cmd);
}


void CCore::slotRecvNonceReply(QNetworkReply* _reply)
{
    if (_reply){
        QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
        qDebug() << "Nonce reply = " << reply;
        m_nonce = reply["value"].toString();
    } else {
        qDebug() << "NULL nonce reply";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvRegisterReply(QNetworkReply* _reply)
{
    quint32 errorCode = _reply->error();
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Registration reply = " << reply;
    QString strStatus = (errorCode==200 && reply["status"] == "ok")?
                "Registration OK!" :
                ("Error " + QString::number(errorCode)) ;
    bool isRegOk = (reply["status"] == "ok") ? true : false;
    emit(signalRegistrationStatus(strStatus,isRegOk));
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvGetTokenReply(QNetworkReply* _reply)
{
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Token reply = " << reply;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200 &&
            reply["status"] == "ok" )
    {
        m_token = reply["token"].toString();
    }
    else{
        qDebug() << "get token recv reply ERROR";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvLoginReply(QNetworkReply* _reply)
{
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Login reply = " << reply;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200 &&
            reply["status"] == "ok" )
    {
        m_token = reply["token"].toString();
        emit(signalLoginStatus("Login OK!", true));
        slotSendRequestItemsCount(10);
        slotSendRequestRecommendItemsCount(15);
    }
    else{
        qDebug() << "get token LOGIN reply ERROR = " << _reply;
        emit(signalLoginStatus("Login Error!", false));
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvItemsCountReply(QNetworkReply* _reply)
{
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Items get count reply = " << reply;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200)
    {
        QJsonArray itemsArr = reply["items"].toArray();
        bool status = false;
        quint32 newItems = 0;
        for (QJsonArray::iterator itr = itemsArr.begin(); itr != itemsArr.end(); itr++){
            qint64 value = (*itr).toString().toLongLong(&status);
            if (status){
                if (!m_itemsIDTotal.contains(value)){
                    newItems++;
                    m_itemsIDFresh.insert( value );
                    CommandItemDescriptionRequest* cmd =
                            new CommandItemDescriptionRequest(&m_serverName,
                                                              &m_token,
                                                              value);
                    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
                            this, SLOT(slotRecvItemInfoReply(QNetworkReply*)));
                    addNetworkCommand(cmd);
                }
            }
        }
        //TODO подумать что надо сделать, если приходят одни и те же элементы
        //        if (newItems == 0){
        //            slotSendRequestItemsCount(7);
        //        }
    }
    else{
        qDebug() << "get Items IDs recv reply ERROR";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvRecommendItemsCountReply(QNetworkReply* _reply)
{
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Items recommend get count reply = " << reply;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200)
    {
        QJsonArray itemsArr = reply["items"].toArray();
        bool status = false;
        quint32 newItems = 0;
        for (QJsonArray::iterator itr = itemsArr.begin(); itr != itemsArr.end(); itr++){
            qint64 value = (*itr).toString().toLongLong(&status);
            if (status){
                if (!m_recommendItemsIDTotal.contains(value)){
                    newItems++;
                    m_recommendItemsIDFresh.insert( value );
                    //TODO made difference between RATE and RECOMEND info
                    CommandItemDescriptionRequest* cmd =
                            new CommandItemDescriptionRequest(&m_serverName,
                                                              &m_token,
                                                              value);
                    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
                            this, SLOT(slotRecvRecommendItemInfoReply(QNetworkReply*)));
                    addNetworkCommand(cmd);
                }
            }
        }
        //TODO подумать что надо сделать, если приходят одни и те же элементы
        //        if (newItems == 0){
        //            slotSendRequestRecommendItemsCount(3);
        //        }
    }
    else{
        qDebug() << "get Recommend Items IDs recv reply ERROR";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvItemInfoReply(QNetworkReply* _reply)
{
    static int counter = 0;
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Item Info get reply = " << reply << " TID = " << QThread::currentThreadId() << " REPLY INFO COUNTER = " << counter++;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200)
    {
        qint64 id = reply["itemId"].toString().toLongLong();
        if (m_itemsIDFresh.contains(id)){
            recvNewDescription(m_itemsRateModel, &reply);
//            emit( signalReceiveNewRateDescription(QString(QJsonDocument(reply).toJson())) );
            emit signalReceiveNewRateDescription("");
            m_itemsIDFresh.remove(id);
            m_itemsIDTotal.insert(id);
        }
    }
    else{
        qDebug() << "get Item Info recv reply ERROR";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvRecommendItemInfoReply(QNetworkReply* _reply)
{
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Recommend Item Info get reply = " << reply;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200)
    {
        qint64 id = reply["itemId"].toString().toLongLong();
        if (m_recommendItemsIDFresh.contains(id)){
//            emit( signalReceiveNewRecommendDescription(QString(QJsonDocument(reply).toJson())) );
            recvNewDescription(m_itemsRecommendModel, &reply);
            emit signalReceiveNewRateDescription("");
            m_recommendItemsIDFresh.remove(id);
            m_recommendItemsIDTotal.insert(id);
        }
    }
    else{
        qDebug() << "get Recommend Item Info recv reply ERROR";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}


void CCore::slotRecvRateItemReply(QNetworkReply* _reply)
{
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Rate item reply = " << reply;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200)
    {
        qDebug() << "slot recv Rate Item reply OK!";
    }
    else{
        qDebug() << "get Rate Item recv reply ERROR";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}

void CCore::slotRecvSkipItemReply(QNetworkReply* _reply)
{
    QJsonObject reply = getJSONObjectFromNetworkReply(_reply);
    qDebug() << "Skip item reply = " << reply;
    quint32 errorCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (_reply->error() == QNetworkReply::NoError &&
            errorCode == 200)
    {
        qDebug() << "slot recv Skip Item reply OK!";
    }
    else{
        qDebug() << "get Skip Item recv reply ERROR";
    }
    emit signalDeleteCommand( (AbstractCommand*)sender() );
}


void CCore::slotSendNonceRequest()
{
    CommandGetNonce* cmd = new CommandGetNonce(&m_serverName);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvNonceReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}

void CCore::slotSendRegistrationRequest(QString login,QString password)
{
    m_login = login;
    m_password = password;
    CommandRegister* cmd = new CommandRegister(&m_serverName,
                                               &m_nonce,
                                               &m_devKey,
                                               &m_login,
                                               &m_password);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvRegisterReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}

void CCore::slotSendLoginRequest(QString login,QString password)
{
    m_login = login;
    m_password = password;
    CommandLogin* cmd = new CommandLogin(&m_serverName,
                                         &m_nonce,
                                         &m_devKey,
                                         &m_login,
                                         &m_password);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvLoginReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}


void CCore::slotSendRequestItemsCount(quint32 count)
{
    CommandItemsCountRequest* cmd = new CommandItemsCountRequest(&m_serverName,
                                                                 &m_token,
                                                                 count);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvItemsCountReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}

void CCore::slotSendRequestRecommendItemsCount(quint32 count)
{
    CommandRecommendItemsCountRequest* cmd = new CommandRecommendItemsCountRequest(&m_serverName,
                                                                                   &m_token,
                                                                                   count);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvRecommendItemsCountReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}

void CCore::slotSendRateItem(qint32 index, qreal rateValue)
{
    quint32 itemId = 0;
    m_itemsRateModel->setData(m_itemsRateModel->index(index), rateValue, ItemsModel::UserRateRole);
    itemId = m_itemsRateModel->data(m_itemsRateModel->index(index), ItemsModel::ItemIDRole).toUInt();
    qDebug() << "Rate in CORE! index = " << index << " ID = " << itemId << " rate = " << rateValue;
    CommandRateItem* cmd = new CommandRateItem(&m_serverName,
                                               &m_token,
                                               itemId,
                                               rateValue);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvRateItemReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}

void CCore::slotSendRateRecommendItem(qint32 index, qreal rateValue)
{
    quint32 itemId = 0;
    m_itemsRecommendModel->setData(m_itemsRateModel->index(index), rateValue, ItemsModel::UserRateRole);
    itemId = m_itemsRecommendModel->data(m_itemsRateModel->index(index), ItemsModel::ItemIDRole).toUInt();
    qDebug() << "Rate in CORE! index = " << index << " ID = " << itemId << " rate = " << rateValue;
    CommandRateItem* cmd = new CommandRateItem(&m_serverName,
                                               &m_token,
                                               itemId,
                                               rateValue);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvRateItemReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}

void CCore::slotSendSkipItem(qint32 index)
{
    m_itemsRateModel->setData(m_itemsRateModel->index(index), true, ItemsModel::UserSkipRole);
    quint32 itemId = m_itemsRateModel->data(m_itemsRateModel->index(index), ItemsModel::ItemIDRole).toUInt();
    qDebug() << "SEND skip item from CORE! index = " << index  << " ItemID = " << itemId;
    CommandSkipItem* cmd = new CommandSkipItem(&m_serverName,
                                               &m_token,
                                               itemId);
    connect(cmd, SIGNAL(signalReply(QNetworkReply*)),
            this, SLOT(slotRecvSkipItemReply(QNetworkReply*)));
    addNetworkCommand(cmd);
}

void CCore::slotCmdError(QString err)
{
    qDebug() << "COMMAND ERROR!!!!!!! = " << err;
    AbstractCommand* cmd = dynamic_cast<AbstractCommand*>(sender());
    if (cmd){
        //TODO верунть удаление какахг
//        emit signalDeleteCommand(cmd);
    }
}

void CCore::destroy()
{
    if (!m_instance) return;
    delete m_instance;
    m_instance = NULL;
}

void CCore::recvNewDescription(ItemsModel* model, QJsonObject* jsdescr)
{
    Item* item = new Item;
    item->setItemID( (*jsdescr)["itemId"].toString().toInt() );
    item->setName((*jsdescr)["name"].toString());
    item->setOriginalName((*jsdescr)["intname"].toString());
    item->setDescription((*jsdescr)["description"].toString());
    item->setPoster((*jsdescr)["poster"].toString());
    model->addItem(item);
}



CCore::~CCore()
{
    qDebug() << "CCore destructor";
    CommandMgr::destroy();
}

