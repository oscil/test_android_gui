#include "commandmgr.h"

CommandMgr* CommandMgr::m_instance = NULL;
QList<AbstractCommand*> CommandMgr::m_commandsFresh;
QSet<AbstractCommand*> CommandMgr::m_commandsExecuted;
QWaitCondition CommandMgr::m_waitCondition;
QMutex CommandMgr::m_waitMutex;
QMutex CommandMgr::m_cmdListMutex(QMutex::Recursive);
bool CommandMgr::m_runExec = true;

CommandMgr::CommandMgr():
    m_commandLoop(this)
{ 
    m_commandLoop.setAutoDelete(false);
    QThreadPool::globalInstance()->start(&m_commandLoop);
    start();
    moveToThread(this);
}

CommandMgr* CommandMgr::instance()
{
    if (m_instance == NULL){
        m_instance = new CommandMgr;
    }
    return m_instance;
}


void CommandMgr::pushCommand(AbstractCommand* cmd)
{
    cmd->activate(this->thread());
    connect(cmd, SIGNAL(signalRepeatCommand(AbstractCommand*)),
            this, SLOT(slotRepushCommand(AbstractCommand*)));
    connect(cmd, SIGNAL(signalCommandObsolete(AbstractCommand*)),
            this, SLOT(slotDeleteCommand(AbstractCommand*)));
    m_cmdListMutex.lock();
    m_commandsFresh.push_back(cmd);
    m_cmdListMutex.unlock();
    m_waitCondition.wakeOne();
}

void CommandMgr::slotRepushCommand(AbstractCommand* cmd)
{
    //TODO guard DATA!!! no thread safety
    m_cmdListMutex.lock();
    if (cmd->repeatType() != AbstractCommand::REPEAT_INFINITELY &&
        cmd->repeatCount() - cmd->executeCount() <= 0)
    {
        m_commandsFresh.removeOne(cmd);
        cmd->stopRepeat();
    }
    else{
        m_commandsFresh.push_back(cmd);
        m_waitCondition.wakeOne();
    }
    m_cmdListMutex.unlock();
}


void CommandMgr::slotDeleteCommand(AbstractCommand* cmd)
{
    qDebug() << "Slot delete command tid = " << QThread::currentThreadId();
    m_cmdListMutex.lock();
    cmd->disconnect(this);
    m_commandsFresh.removeOne(cmd);
    m_commandsExecuted.erase( m_commandsExecuted.find(cmd) );
    //TODO was just delete cmd
    delete cmd;
    m_cmdListMutex.unlock();
}

void CommandMgr::slotOccuredEvent(AbstractEvent* event)
{
    qDebug() << "Mgr slot occurs event type = " << event->eventType();
    m_cmdListMutex.lock();
    if (!m_commandsExecuted.isEmpty()){
        QSet<AbstractCommand*>::iterator itr = m_commandsExecuted.begin();
        while(itr != m_commandsExecuted.end()){
            if ( (*itr)->check(event) ){
                m_commandsFresh.removeOne(*itr);
                itr = m_commandsExecuted.erase(itr);
                continue;
                //break; //если событие закрывает несколько команд - то несколько команд закрываются ОДНИМ событием
            }
            itr++;
        }
    }
    m_cmdListMutex.unlock();
    delete event;
}

void CommandMgr::destroy()
{
    if (!m_instance) return;
    m_runExec = false;
    m_waitCondition.wakeAll();
    qDebug() << "ComMgr after wake all";
    QThreadPool::globalInstance()->waitForDone();
    qDebug() << "ComMgr before delete";
    m_instance->quit();
    delete m_instance;
    m_instance = NULL;
}

CommandMgr::~CommandMgr()
{
    qDebug() << "Command Mgr destructor";
}

