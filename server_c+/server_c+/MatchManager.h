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
	bool Push_Back(ClientSession* client);

	// return wait count
	int GetWaitCount() { return mMatchWaitList.size(); }

	// ��Ī�� ����ϴ� Thread ������ ���ư�
	// Match List ���� 2���̻� ������� ��� ��Ī�Ϸ� �� ���� ����
	void ProcessMatchList();

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	// ��Ī ��⿭�� ���� ����Ʈ
	std::list<ClientSession*> mMatchWaitList;

	// for thread-safe
	CRITICAL_SECTION mCS;
};

MatchManager* TemplateSingleton<MatchManager>::m_pInstance = NULL;