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
    timer = new QElapsedTimer();
    timer->start();
}
void Timer::stop()
{
    //m_lastFrameTime = SDL_GetTicks() - m_startTicks;
    m_lastFrameTime = timer->nsecsElapsed() / 1000;
    delete timer;
	m_accumulatedTime += m_lastFrameTime / 1000.0f;
}
unsigned int Timer::getFrameTime()
{
	return m_lastFrameTime;
}

bool Timer::canGetTimeChunk()
{
	return m_accumulatedTime > TIME_CHUNK;
}
float Timer::getTimeChunk()
{
	m_accumulatedTime -= TIME_CHUNK;
	return TIME_CHUNK;
}
