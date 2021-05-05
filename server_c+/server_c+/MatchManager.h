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
	int GetWaitCount() { return m_MatchWaitList.size(); }

	// ��Ī�� ����ϴ� Thread ������ ���ư�
	// Match List ���� 2���̻� ������� ��� ��Ī�Ϸ� �� ���� ����
	void ProcessMatchList();

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&m_CS); }
	void LeaveCS() { LeaveCriticalSection(&m_CS); }

	void SetIsStop(bool isStop) { m_IsStop = isStop; }
private:
	// ��Ī ��⿭�� ���� ����Ʈ
	std::list<ClientSession*> m_MatchWaitList;

	// for thread-safe
	CRITICAL_SECTION m_CS;

	// while loop stop flag
	bool m_IsStop;
};

MatchManager* TemplateSingleton<MatchManager>::m_pInstance = NULL;