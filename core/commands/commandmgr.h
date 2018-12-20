#ifndef COMMANDMGR_H
#define COMMANDMGR_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QRunnable>
#include <QThreadPool>
#include <QSet>

#include "core/commands/abstractcommand.h"

class CommandTimer;


class CommandMgr : public QThread
{
    Q_OBJECT
public:
    static CommandMgr* instance();
    static void destroy();
    void pushCommand(AbstractCommand*);
    ~CommandMgr();

public slots:
    void slotRepushCommand(AbstractCommand*);
    void slotDeleteCommand(AbstractCommand*);
    void slotOccuredEvent(AbstractEvent*);

private:
    static QList<AbstractCommand*> m_commandsFresh;
    static QSet<AbstractCommand*> m_commandsExecuted;
    static QWaitCondition m_waitCondition;
    static QMutex m_waitMutex;
    static QMutex m_cmdListMutex;
    static CommandMgr* m_instance;
    static bool m_runExec;

    CommandMgr();

    class CommandLoop : public QRunnable{
    public:
        inline CommandLoop(CommandMgr* mgr):
            m_mgr(mgr)
        {
        }
        inline void run(){
            while(m_mgr->m_runExec){
                m_mgr->m_waitMutex.lock();
                m_mgr->m_waitCondition.wait(&m_waitMutex);
//                qDebug() << "Command LOOP FRESH = " << m_commandsFresh.size() <<
//                            " OLD = " << m_commandsExecuted.size() + "------------------------";
//                foreach (QObject* obj, m_commandsFresh) {
//                    qDebug() << "FRESH = " << obj->objectName();
//                }
//                foreach (QObject* obj, m_commandsExecuted) {
//                    qDebug() << "OLD = " << obj->objectName();
//                }
//                qDebug() << "--------------------------------------------------";
                if (m_commandsFresh.isEmpty()){
                    m_mgr->m_waitMutex.unlock();
                    continue;
                }

                while(!m_commandsFresh.isEmpty()){
                    m_mgr->m_cmdListMutex.lock();
                    AbstractCommand* cmd = m_commandsFresh.takeFirst();
                    m_commandsExecuted.insert(cmd);
                    m_mgr->m_cmdListMutex.unlock();
                    cmd->execute();
                }
                    m_mgr->m_waitMutex.unlock();
            }
        }

    private:
        CommandMgr* m_mgr;
    } m_commandLoop;


};

#endif // COMMANDMGR_H
