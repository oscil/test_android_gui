#ifndef EVENTS_H
#define EVENTS_H

#include <QNetworkReply>

class AbstractEvent{
public:
    enum EventType{EVENT_NETWORK_REPLY, EVENT_DEBUG_SMTH};
    inline AbstractEvent(EventType type):
    m_type(type)
    {
    }
    inline EventType eventType(){return m_type;}
    inline virtual ~AbstractEvent() {}
protected:
    EventType m_type;
};

class NetworkReplyEvent : public AbstractEvent{
public:
    inline NetworkReplyEvent(QNetworkReply* reply):
        AbstractEvent(EVENT_NETWORK_REPLY),
        m_reply(reply)
    {
    }
    inline QNetworkReply* getNetworkReply(){
        return m_reply;
    }
    //TODO delete network reply
private:
    QNetworkReply* m_reply;
};

class DebugSmthEvent : public AbstractEvent{
public:
    inline DebugSmthEvent(QString str):
        AbstractEvent(EVENT_DEBUG_SMTH),
        m_str(str)
    {
    }
    inline QString* getDebugSmth(){
        return &m_str;
    }
private:
    QString m_str;

};

#endif // EVENTS_H

