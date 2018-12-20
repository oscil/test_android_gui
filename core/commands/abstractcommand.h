#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <qglobal.h>
#include <QObject>
#include <QTimer>
#include <QThread>
#include "core/commands/events.h"

class AbstractCommand;

class AbstractProcessor : public QObject{
    Q_OBJECT
public:
    virtual void process(AbstractEvent* event) = 0;
    virtual inline ~AbstractProcessor(){
    }
};

class AbstractChecker{
public:
    virtual bool check(AbstractEvent* event) = 0;
    virtual inline ~AbstractChecker(){}
};


class AbstractCommand : public QObject
{
    Q_OBJECT
public:
    enum CommandRepeateType {REPEAT_INFINITELY, REPEAT_ONCE, REPEAT_COUNT};
    inline AbstractCommand(AbstractChecker* checker,
                           AbstractProcessor* processor):
        m_checker(checker),
        m_processor(processor),
        m_msecRepeatInterval(10000),
        m_repeatType(REPEAT_ONCE),
        m_repeatCount(1),
        m_triggerEventType(AbstractEvent::EVENT_NETWORK_REPLY),
        m_executeCount(0),
        m_timerRepeat(NULL),
        m_timerObsolete(NULL),
        m_mseclifetime(0)
    {
    }
    virtual void execute();
    inline bool check(AbstractEvent* event) {
        if (event->eventType() != m_triggerEventType)
            return false;
        bool check = m_checker->check(event);
        if (check){
            m_processor->process(event);
        }
        return check;
    }

    inline void setRepeatInterval(quint32 msecInterval){
        m_msecRepeatInterval = msecInterval;
    }

    inline quint32 repeatInterval(){
        return m_msecRepeatInterval;
    }

    inline void setRepeatType(CommandRepeateType repeatType, qint32 count = 1){
        m_repeatType = repeatType;
        if (repeatType == REPEAT_ONCE) {
            m_repeatCount = 1;
        } else if (repeatType == REPEAT_COUNT){
            m_repeatCount = count;
        }
    }

    inline void setRepeatCount(qint32 count){
        if (count == 1)
            m_repeatType = REPEAT_ONCE;
        m_repeatCount = count;
    }

    inline CommandRepeateType repeatType(){
        return m_repeatType;
    }

    inline qint32 repeatCount(){
        return m_repeatCount;
    }

    inline void setEventType (AbstractEvent::EventType type){
        m_triggerEventType = type;
    }

    inline qint32 executeCount(){
        return m_executeCount;
    }

    inline void setLifeTime(quint32 t){
        m_mseclifetime = t;
    }

    inline void stopRepeat(){
        qDebug() << "Stop repeat tid = " << QThread::currentThreadId();
        if (m_timerRepeat)
            m_timerRepeat->stop();
        if (m_timerObsolete)
            m_timerObsolete->start();
    }

    inline void activate(QThread* timerThread){
        qDebug() << "Command activate tid = " << QThread::currentThreadId();
        if (repeatCount() > 1 || repeatType() == REPEAT_INFINITELY){
            m_timerRepeat = new QTimer;
            connect(m_timerRepeat, SIGNAL(timeout()),
                    this, SLOT(slotTimedOutRepeater()));
            m_timerRepeat->start( repeatInterval() );
            m_timerRepeat->moveToThread(timerThread);
        }
        if (m_mseclifetime > 0){
            m_timerObsolete = new QTimer;
            connect(m_timerObsolete, SIGNAL(timeout()),
                    this, SLOT(slotTimedOutObsolete()));
            m_timerObsolete->setSingleShot(true);
            m_timerObsolete->setInterval(m_mseclifetime);
            m_timerObsolete->moveToThread(timerThread);
        }
    }

    virtual ~AbstractCommand();

protected:
    AbstractChecker* m_checker;
    AbstractProcessor* m_processor;
    quint32 m_msecRepeatInterval;
    CommandRepeateType m_repeatType;
    qint32 m_repeatCount;
    AbstractEvent::EventType m_triggerEventType;
    qint32 m_executeCount;
    QTimer* m_timerRepeat;
    QTimer* m_timerObsolete;
    quint32 m_mseclifetime;
    static int m_count;

private slots:
    inline void slotTimedOutRepeater(){
        emit signalRepeatCommand(this);
    }
    inline void slotTimedOutObsolete(){
        emit signalCommandObsolete(this);
    }

signals:
    void signalRepeatCommand(AbstractCommand*);
    void signalCommandObsolete(AbstractCommand*);
    void signalCommandError(QString);

};

#endif // ABSTRACTCOMMAND_H
