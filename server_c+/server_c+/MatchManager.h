#pragma once

#include "stdafx.h"
#include <list>
#include "ClientSession.h"
#include "TemplateSingleton.h"
#include "PacketList.h"
#include "PacketManager.h"

/*
�÷��̾ 1:1 ��ġ�� ��ٸ��� ��Ī����
��ġ�� ���� ������� List �� push.
��Ī ��⸦ ����� �� �ֱ⶧���� Queue ���� List �� �����Ѵ�.
*/
class MatchManager : public TemplateSingleton<MatchManager>
{
public:
	MatchManager();
	~MatchManager();

	void Init() override;
	void Clean() override;
	
	// push back to list for wait play
	bool PushBackClient(ClientSession* client);
	bool CheckExistClient(ClientSession* client);
	bool DeleteClient(ClientSession* client);

	// return wait count
	int GetWaitCount() { return mMatchWaitList.size(); }

	// ��Ī�� ����ϴ� Thread ������ ���ư�
	// Match List ���� 2���̻� ������� ��� ��Ī�Ϸ� �� ���� ����
	void ProcessMatchList();

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }

	void SetStopFlag(bool stopFlag) { mStopFlag = stopFlag; }
private:
	// ��Ī ��⿭�� ���� ����Ʈ
	std::list<ClientSession*> mMatchWaitList;

	// for thread-safe
	CRITICAL_SECTION mCS;

	// while loop stop flag
	bool mStopFlag;
};

MatchManager* TemplateSingleton<MatchManager>::m_pInstance = NULL;