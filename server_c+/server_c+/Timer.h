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
	// �Ϲ� �ð�
	struct tm m_Time;
	time_t m_LongTime;

	// Ÿ�̸� �뵵
	clock_t m_iStartClock, m_iEndClock;
};

