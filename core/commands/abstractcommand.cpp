#include "abstractcommand.h"

int AbstractCommand::m_count = 0;

void AbstractCommand::execute()
{
    m_count++;
    m_executeCount++;
}

AbstractCommand::~AbstractCommand()
{
    m_count--;
    qDebug() << "After delete command count = " << m_count;
    delete m_checker;
    m_processor->disconnect(this);
    m_processor->deleteLater();
    if (m_timerRepeat != NULL){
        m_timerRepeat->stop();
        m_timerRepeat->disconnect(this);
        delete m_timerRepeat;
        m_timerRepeat = NULL;
    }
    if (m_timerObsolete != NULL){
        m_timerObsolete->stop();
        m_timerObsolete->disconnect(this);
        delete m_timerObsolete;
        m_timerObsolete = NULL;
    }
}
