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

	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }

	void Init();
	void Clean();

	bool InGame(ClientSession* player1, ClientSession* player2);	
	bool OutGame(int roomNum);

	bool GameEnd(int roomNum, GAMEEND_STATE endState);

	InGameRoom* SearchRoom(int roomNum);

	ClientSession* GetEnemyClient(ClientSession* player);	

	// 플레이어 정보를 받아서 변경 후 변경된 정보를 구조체에 담아 다시 반환
	SCHARACTER SetPlayer(ClientSession* player, SCHARACTER charPacket);
private:
	std::list<InGameRoom*> mInGameRoomList;

	int mRoomCount;
	int mLastRoomNum;

	CRITICAL_SECTION mCS;
};

