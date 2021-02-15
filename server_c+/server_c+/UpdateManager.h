#pragma once

#include "FpsDelayChecker.h"

#define UPDATE_FRAME 60

class UpdateManager
{
public:

	UpdateManager();
	~UpdateManager();

	bool Update();

private:

	FpsDelayChecker m_DelayChecker;

};

extern UpdateManager* g_pUpdateManager;