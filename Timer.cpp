#include "Timer.h"
#include <SDL2/SDL.h>

Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::start()
{
	m_startTicks = SDL_GetTicks();
}
void Timer::stop()
{
	m_lastFrameTime = SDL_GetTicks() - m_startTicks;
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
