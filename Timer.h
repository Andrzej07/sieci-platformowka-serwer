#pragma once
class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void stop();
	unsigned int getFrameTime();
	bool canGetTimeChunk();
	float getTimeChunk();

private:
	float m_accumulatedTime = 0.0f;
	const float TIME_CHUNK = 16.66f / 2000.0f;

	unsigned int m_startTicks = 0;
	unsigned int m_lastFrameTime = 1;
};

