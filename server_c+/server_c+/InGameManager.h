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

	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }

	void Init();
	void Clean();

	bool InGame(ClientSession* player1, ClientSession* player2);	
	bool OutGame(int roomNum);

	bool GameEnd(int roomNum, GAMEEND_STATE endState);

	InGameRoom* SearchRoom(int roomNum);

	ClientSession* GetEnemyClient(ClientSession* player);	

	// �÷��̾� ������ �޾Ƽ� ���� �� ����� ������ ����ü�� ��� �ٽ� ��ȯ
	SCHARACTER SetPlayer(ClientSession* player, SCHARACTER charPacket);
private:
	std::list<InGameRoom*> mInGameRoomList;

	int mRoomCount;
	int mLastRoomNum;

	CRITICAL_SECTION mCS;
};

