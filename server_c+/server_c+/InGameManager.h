#pragma once
#include "stdafx.h"
#include <list>
#include "PacketList.h"

#include "TemplateSingleton.h"
#include "ClientSession.h"
#include "PlayerInfo.h"
#include "PacketManager.h"
#include "InGameRoom.h"

/*
현재 대전 중인 플레이어를 짝지어 리스트로 저장
따로 쓰레드를 통한 프로세싱은 필요하지 않으며
대전 중인 ClientSession 정보를 관리하며
Send() / Recv() 시에 상대방에게 정보를 보내기위해 주로 사용한다.
*/
class InGameManager : public TemplateSingleton<InGameManager>
{
public:
	InGameManager();
	~InGameManager();

	// PacketManager -> InGameManager : 인게임로직에 관련한 패킷은 InGameManager 에서 처리
	bool Enqueue(ClientPacket clientPacket);
	bool Dequeue();

	void ProcessAllQueue();

	PROTOCOL ParsingPacket(ClientPacket pack);

	void ProcessPacket(PROTOCOL protocol, ClientPacket pack);

	void EnterCS() { EnterCriticalSection(&m_CS); }
	void LeaveCS() { LeaveCriticalSection(&m_CS); }

	void Init();
	void Clean();

	bool InGame(ClientSession* player1, ClientSession* player2);	
	bool OutGame(int roomNum);
	bool GameEnd(int roomNum, GAMEEND_STATE endState);

	InGameRoom* SearchRoom(int roomNum);

	bool DeleteRoomInList(int roomNum);

	ClientSession* GetEnemyClient(ClientSession* player);	

	// 플레이어 정보를 받아서 변경 후 변경된 정보를 구조체에 담아 다시 반환
	SCHARACTER SetPlayer(ClientSession* player, SCHARACTER charPacket);

	void SetIsStop(bool isStop) { m_IsStop = isStop; }

	std::list<InGameRoom*>* GetRoomListPtr() { return &m_InGameRoomList; }
private:
	std::list<InGameRoom*> m_InGameRoomList;

	std::vector<std::list<InGameRoom*>> m_ListVector;

	// 게임 내에서 처리가 필요한 패킷의 경우 이 큐를 통해 처리
	queue<ClientPacket> m_InGameBufferQueue;

	int m_RoomCount;
	int m_LastRoomNum;

	CRITICAL_SECTION m_CS;

	bool m_IsStop;
};

