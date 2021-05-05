#include "FpsDelayChecker.h"

FpsDelayChecker::FpsDelayChecker(int fps)
	:
	m_ResultDelayTime(0),
	m_QuotientClock(0),
	m_RemainClock(0.0f),
	m_TotalRemainTime(0.0f)
{
	init(fps);
}

FpsDelayChecker::~FpsDelayChecker()
{
}

// private functions /////////////////////////////

void FpsDelayChecker::init(int fps)
{
	float calcResult = 1000.00f / (float)fps;

	m_QuotientClock = (int)calcResult;
	m_RemainClock = calcResult - m_QuotientClock;
}

void FpsDelayChecker::calcDelayTime()
{
	m_ResultDelayTime = m_QuotientClock;

	m_TotalRemainTime += m_RemainClock;

	if (m_TotalRemainTime >= 1)
	{
		m_ResultDelayTime += 1;
		m_TotalRemainTime -= 1.0f;
	}

	m_ResultDelayTime -= getCodeRuntime();
}

DWORD FpsDelayChecker::getCodeRuntime()
{
	return (DWORD)m_Timer.GetClockOffset();
}

//////////////////////////////////////////////////

void FpsDelayChecker::DoDelay()
{
	calcDelayTime();

	Sleep(m_ResultDelayTime);
}

void FpsDelayChecker::StartCodeRuntimeCheck()
{
	m_Timer.StartClock();
}

void FpsDelayChecker::EndCodeRuntimeCheck()
{
	m_Timer.EndClock();
}