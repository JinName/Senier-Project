#pragma once

#include "FpsDelayChecker.h"

#define SYNC_FPS 60

class ClientSyncManager
{
public:

	ClientSyncManager();
	~ClientSyncManager();

	bool Synchronization();

private:

	FpsDelayChecker m_DelayChecker;

	bool m_bUpdateSwitch;

};

extern ClientSyncManager* g_pClientSyncManager;