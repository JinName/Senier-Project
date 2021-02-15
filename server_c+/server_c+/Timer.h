#pragma once

#include <ctime>
#include <time.h>

class Timer
{
public:
	Timer();
	~Timer();

	void SetCurrentLocalTime();

	int GetHour();
	int GetMin();
	int GetSec();

	void StartClock();
	void EndClock();
	void ResetClock();

	clock_t GetClockOffset();
	float GetClockOffsetByFloat();

private:
	// 일반 시간
	struct tm m_Time;
	time_t m_LongTime;

	// 타이머 용도
	clock_t m_iStartClock, m_iEndClock;
};

