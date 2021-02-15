#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::SetCurrentLocalTime()
{
	time(&m_LongTime);						// 시간을 얻어와서
	localtime_s(&m_Time, &m_LongTime);		// local time으로 전환
}

int Timer::GetHour()
{
	return m_Time.tm_hour;
}

int Timer::GetMin()
{
	return m_Time.tm_min;
}

int Timer::GetSec()
{
	return m_Time.tm_sec;
}

void Timer::StartClock()
{
	m_iStartClock = clock();
}

void Timer::EndClock()
{
	m_iEndClock = clock();
}

void Timer::ResetClock()
{
	m_iStartClock = 0;
	m_iEndClock = 0;
}

clock_t Timer::GetClockOffset()
{
	return m_iEndClock - m_iStartClock;
}

float Timer::GetClockOffsetByFloat()
{
	return ((float)(m_iEndClock - m_iStartClock) / CLOCKS_PER_SEC);
}