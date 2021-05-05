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
	// ����ȭ �ڵ� ���� �ð� üũ
	m_DelayChecker.StartCodeRuntimeCheck();

	// ����ġ�� true �� ���� Ŭ���̾�Ʈ ����ȭ
	if (m_bUpdateSwitch)
	{
		// ������ ���� ������ �� Ŭ���̾�Ʈ�� ����

		// ����ȭ ������ ����ġ false
		m_bUpdateSwitch = false;
	}	

	m_DelayChecker.EndCodeRuntimeCheck();
	
	m_DelayChecker.DoDelay();

	return true;
}