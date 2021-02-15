#include "FpsDelayChecker.h"

FpsDelayChecker::FpsDelayChecker(int _fps)
	:
	m_dwResultDelayTime(0),
	m_iQuotientClock(0),
	m_fRemainClock(0.0f),
	m_fTotalRemainTime(0.0f)
{
	init(_fps);
}

FpsDelayChecker::~FpsDelayChecker()
{
}

// private functions /////////////////////////////

void FpsDelayChecker::init(int _fps)
{
	float calcResult = 1000.00f / (float)_fps;

	m_iQuotientClock = (int)calcResult;
	m_fRemainClock = calcResult - m_iQuotientClock;
}

void FpsDelayChecker::calcDelayTime()
{
	m_dwResultDelayTime = m_iQuotientClock;

	m_fTotalRemainTime += m_fRemainClock;

	if (m_fTotalRemainTime >= 1)
	{
		m_dwResultDelayTime += 1;
		m_fTotalRemainTime -= 1.0f;
	}

	m_dwResultDelayTime -= getCodeRuntime();
}

DWORD FpsDelayChecker::getCodeRuntime()
{
	return (DWORD)m_Timer.GetClockOffset();
}

//////////////////////////////////////////////////

void FpsDelayChecker::DoDelay()
{
	calcDelayTime();

	Sleep(m_dwResultDelayTime);
}

void FpsDelayChecker::StartCodeRuntimeCheck()
{
	m_Timer.StartClock();
}

void FpsDelayChecker::EndCodeRuntimeCheck()
{
	m_Timer.EndClock();
}