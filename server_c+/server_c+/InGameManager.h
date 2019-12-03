#pragma once
#include "stdafx.h"
#include <list>

#include "TemplateSingleton.h"
#include "ClientSession.h"
#include "PlayerInfo.h"

typedef struct sInGameRoom
{
	ClientSession* mPlayer1;	
	ClientSession* mPlayer2;
	//PlayerInfo mPlayerInfo[2];
	int mRoomNum;
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

	ClientSession* GetEnemyClient(ClientSession* player);

	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	std::list<SINGAMEROOM> mInGameRoomContainer;

	int mRoomCount;
	int mLastRoomNum;

	CRITICAL_SECTION mCS;
};

