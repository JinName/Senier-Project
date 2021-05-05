#pragma once

#include "FpsDelayChecker.h"
#include <thread>

#define SYNC_FPS 30

// update thread pool ����
// -> �ϳ��� ������ ���� ������Ʈ�� ����� ���� ������ ���� ex. ������ 1�� �� gameroom 200��
// -> ���ӷ��� ������ �Ѿ������ �����带 �ϳ��� ����

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