#pragma once

#include "stdafx.h"
#include <list>
#include "ClientSession.h"
#include "TemplateSingleton.h"
#include "PacketList.h"
#include "PacketManager.h"

/*
플레이어가 1:1 매치를 기다리는 매칭서버
매치를 누른 순서대로 List 에 push.
매칭 대기를 취소할 수 있기때문에 Queue 보단 List 를 선택한다.
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

	// 매칭을 담당하는 Thread 내에서 돌아감
	// Match List 내에 2명이상 대기중일 경우 매칭완료 후 게임 시작
	void ProcessMatchList();

	// 편의를 위한 CRITICAL_SECTION 함수
	void EnterCS() { EnterCriticalSection(&m_CS); }
	void LeaveCS() { LeaveCriticalSection(&m_CS); }

	void SetIsStop(bool isStop) { m_IsStop = isStop; }
private:
	// 매칭 대기열을 담을 리스트
	std::list<ClientSession*> m_MatchWaitList;

	// for thread-safe
	CRITICAL_SECTION m_CS;

	// while loop stop flag
	bool m_IsStop;
};

MatchManager* TemplateSingleton<MatchManager>::m_pInstance = NULL;