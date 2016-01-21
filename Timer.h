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
    int getSleepTime();
	bool canGetTimeChunk();
	float getTimeChunk();

private:
	float m_accumulatedTime = 0.0f;
    const int UPDATES_PER_FRAME = 2;
    const float TIME_CHUNK = 16.66f / (1000.0f * UPDATES_PER_FRAME);

    QElapsedTimer *timer;
    qint64 m_lastFrameTime = 1;
    int m_timeChunks = 0;
};

