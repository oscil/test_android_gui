#ifndef ABSTRACTNETWORKCOMMAND_H
#define ABSTRACTNETWORKCOMMAND_H

#include "abstractcommand.h"

class AbstractNetworkProcessor : public AbstractProcessor{
    Q_OBJECT
signals:
    void signalReply(QNetworkReply*);
};

class AbstractNetworkChecker : public AbstractChecker{
public:
    AbstractNetworkChecker(QNetworkReply* = NULL);
    inline void setNetReply(QNetworkReply* rep){m_netReply = rep;}
    inline const QNetworkReply* netReply() const {return m_netReply;}
protected:
    QNetworkReply* m_netReply;
};


class AbstractNetworkCommand : public AbstractCommand
{
    Q_OBJECT
public:
    AbstractNetworkCommand(AbstractNetworkChecker* checker,
                           AbstractNetworkProcessor* processor);
    inline void setReply(QNetworkReply* reply)
    {
        m_reply = reply;
        m_netChecker->setNetReply(reply);
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slotCmdReplyError(QNetworkReply::NetworkError)));
    }
    ~AbstractNetworkCommand();

protected:
    QNetworkReply* m_reply;
    AbstractNetworkChecker* m_netChecker;

signals:
    void signalRequest(QNetworkRequest* request, QByteArray data);
    void signalReply(QNetworkReply*);
    void signalReplyError(QNetworkReply::NetworkError);

public slots:
    inline void slotCmdReplyError(QNetworkReply::NetworkError err){
        emit signalCommandError("Error! net reply. CODE = " + QString::number(err));
    }
};

#endif // ABSTRACTNETWORKCOMMAND_H
