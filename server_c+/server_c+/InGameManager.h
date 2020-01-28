#pragma once
#include "stdafx.h"
#include <list>
#include "PacketList.h"

#include "TemplateSingleton.h"
#include "ClientSession.h"
#include "PlayerInfo.h"
#include "PacketManager.h"

// 유저가 대전하는 방 내의 정보를 담는 구조체
// 해당 룸의 번호(int mRoomNum)
// 어떤 클라이언트가 접속해있는지(ClientSession* mPlayer1, mPlayer2)
// 각 클라이언트의 캐릭터 정보(PlayerInfo mPlayerInfo1, mPlayerInfo2)
// 어떤 맵에 있는지(int mMapNum)
// 해당 룸의 충돌처리를 담당할 충돌 매니저(CollisionManager mCollisionMgr)
typedef struct sInGameRoom
{
	sInGameRoom(ClientSession* player1, ClientSession* player2, int roomNum) : mPlayer1(player1), mPlayer2(player2), mRoomNum(roomNum) {}
	~sInGameRoom() {}

	int mRoomNum;
	ClientSession* mPlayer1;	
	ClientSession* mPlayer2;
	PlayerInfo mPlayerInfo1;
	PlayerInfo mPlayerInfo2;
	int mMapNum;
	
}SINGAMEROOM;

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

	void Init();
	void Clean();

	bool InGame(ClientSession* player1, ClientSession* player2);	
	bool OutGame(int roomNum);

	bool GameEnd(int roomNum, GAMEEND_STATE endState);

	SINGAMEROOM* SearchRoom(int roomNum);

	ClientSession* GetEnemyClient(ClientSession* player);

	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	std::list<SINGAMEROOM*> mInGameRoomContainer;

	int mRoomCount;
	int mLastRoomNum;

	CRITICAL_SECTION mCS;
};

