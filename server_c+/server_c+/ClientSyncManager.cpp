#include "ClientSyncManager.h"

ClientSyncManager* g_pClientSyncManager = nullptr;

ClientSyncManager::ClientSyncManager()
	:
	m_DelayChecker(SYNC_FPS),
	m_bUpdateSwitch(false)
{
}

ClientSyncManager::~ClientSyncManager()
{
}

bool ClientSyncManager::Synchronization()
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