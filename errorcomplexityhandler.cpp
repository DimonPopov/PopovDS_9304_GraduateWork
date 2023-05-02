#include "errorcomplexityhandler.h"



ErrorComplexityHandler::ErrorComplexityHandler(QObject *parent)
    : QObject(parent),
    m_time(0),
    m_timer(new QElapsedTimer)
{
    m_timer->start();
}

void ErrorComplexityHandler::startTimer()
{
    m_timer->restart();
}

void ErrorComplexityHandler::stopTimer()
{
    m_time = m_timer->nsecsElapsed();
}

int ErrorComplexityHandler::getTime(const quint32 &divider) const
{
    return m_time / divider;
}
