#pragma once

#include "FpsDelayChecker.h"
#include <thread>

#define SYNC_FPS 30

// update thread pool 생성
// -> 하나의 스레드 마다 업데이트를 담당할 룸의 갯수를 설정 ex. 스레드 1개 당 gameroom 200개
// -> 게임룸이 단위를 넘어갈때마다 스레드를 하나씩 깨움

class UpdateManager
{
public:

	UpdateManager();
	~UpdateManager();

	bool Synchronization();

private:

	FpsDelayChecker m_DelayChecker;

	bool m_bUpdateSwitch;

};

extern UpdateManager* g_pUpdateManager;