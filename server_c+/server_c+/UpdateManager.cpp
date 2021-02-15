#include "UpdateManager.h"

UpdateManager* g_pUpdateManager = nullptr;

UpdateManager::UpdateManager()
	:
	m_DelayChecker(UPDATE_FRAME)
{
}

UpdateManager::~UpdateManager()
{
}