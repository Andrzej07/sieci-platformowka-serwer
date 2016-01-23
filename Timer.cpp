#include "Timer.h"

Timer::Timer()
{
}


Timer::~Timer()
{
    if(timer)
        delete timer;
}

void Timer::start()
{
    //m_startTicks = SDL_GetTicks();
    m_timeChunks = UPDATES_PER_FRAME;
    timer = new QElapsedTimer();
    timer->start();
}
void Timer::stop()
{
    //m_lastFrameTime = SDL_GetTicks() - m_startTicks;
    /*
    m_lastFrameTime = timer->nsecsElapsed() / 1000;
    delete timer;
	m_accumulatedTime += m_lastFrameTime / 1000.0f;
    */
    m_lastFrameTime = timer->nsecsElapsed() / 1000000.f;
    delete timer;
}
unsigned int Timer::getFrameTime()
{
    return m_lastFrameTime;
}

bool Timer::canGetTimeChunk()
{
    //return m_accumulatedTime > TIME_CHUNK;
    return m_timeChunks>0;
}
float Timer::getTimeChunk()
{
    //m_accumulatedTime -= TIME_CHUNK;
    m_timeChunks--;
	return TIME_CHUNK;
}
int Timer::getSleepTime()
{
    int sleepTime = static_cast<int>(16.666f - m_lastFrameTime);
    return (sleepTime>0) ? sleepTime : 0;
}
