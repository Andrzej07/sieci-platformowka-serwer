#pragma once
#include <QElapsedTimer>
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

    QElapsedTimer *timer;
    qint64 m_lastFrameTime = 1;
};

