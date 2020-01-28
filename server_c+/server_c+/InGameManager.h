#pragma once
#include "stdafx.h"
#include <list>
#include "PacketList.h"

#include "TemplateSingleton.h"
#include "ClientSession.h"
#include "PlayerInfo.h"
#include "PacketManager.h"

// ������ �����ϴ� �� ���� ������ ��� ����ü
// �ش� ���� ��ȣ(int mRoomNum)
// � Ŭ���̾�Ʈ�� �������ִ���(ClientSession* mPlayer1, mPlayer2)
// �� Ŭ���̾�Ʈ�� ĳ���� ����(PlayerInfo mPlayerInfo1, mPlayerInfo2)
// � �ʿ� �ִ���(int mMapNum)
// �ش� ���� �浹ó���� ����� �浹 �Ŵ���(CollisionManager mCollisionMgr)
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
���� ���� ���� �÷��̾ ¦���� ����Ʈ�� ����
���� �����带 ���� ���μ����� �ʿ����� ������
���� ���� ClientSession ������ �����ϸ�
Send() / Recv() �ÿ� ���濡�� ������ ���������� �ַ� ����Ѵ�.
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

