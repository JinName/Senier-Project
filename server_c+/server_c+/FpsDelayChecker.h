#pragma once
#include <Windows.h>
#include <time.h>

#include "Timer.h"

// 원하는 업데이트 주기 (fps)를 입력받아서
// 해당 fps 에 맞는 sleep 주기를 계산해주는 클래스
// 계산된 딜레이로 sleep 까지 수행하는 것이 목표
// 1. 코드 실행타임 체크
// 2. sleep이 불가능한 소수점에 대한 계산

class FpsDelayChecker
{
public:

	FpsDelayChecker(int fps);
	~FpsDelayChecker();

	// 계산된 수치만큼 Sleep() 함수 실행
	void DoDelay();	

	// 코드 실행타임 체크
	void StartCodeRuntimeCheck();
	void EndCodeRuntimeCheck();

private:

	// 코드 실행타임 체크
	Timer m_Timer;

	DWORD m_ResultDelayTime;

	int m_QuotientClock;			// fps 계산 후 몫
	float m_RemainClock;			// fps 계산 후 소수점 나머지

	float m_TotalRemainTime;		// sleep 하고 남은 소수점을 더해서 1이 넘을경우 delaytime + 1

	// 초기화
	void init(int fps);

	// 딜레이 시간 계산
	void calcDelayTime();

	DWORD getCodeRuntime();

};

