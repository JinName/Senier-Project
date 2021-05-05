#include "UpdateManager.h"

UpdateManager* g_pClientSyncManager = nullptr;

UpdateManager::UpdateManager()
	:
	m_DelayChecker(SYNC_FPS),
	m_bUpdateSwitch(false)
{
}

UpdateManager::~UpdateManager()
{
}

bool UpdateManager::Synchronization()
{
	// 동기화 코드 실행 시간 체크
	m_DelayChecker.StartCodeRuntimeCheck();

	// 스위치가 true 일 때만 클라이언트 동기화
	if (m_bUpdateSwitch)
	{
		// 게임의 현재 정보를 각 클라이언트에 전송

		// 동기화 끝나면 스위치 false
		m_bUpdateSwitch = false;
	}	

	m_DelayChecker.EndCodeRuntimeCheck();
	
	m_DelayChecker.DoDelay();

	return true;
}