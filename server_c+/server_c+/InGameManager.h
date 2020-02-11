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

	// PacketManager -> InGameManager : �ΰ��ӷ����� ������ ��Ŷ�� InGameManager ���� ó��
	bool Enqueue(ClientPacket clientPacket);
	bool Dequeue();

	void ProcessAllQueue();

	PROTOCOL ParsingPacket(ClientPacket pack);

	void ProcessPacket(PROTOCOL protocol, ClientPacket pack);

	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }

	void Init();
	void Clean();

	bool InGame(ClientSession* player1, ClientSession* player2);	
	bool OutGame(int roomNum);
	bool GameEnd(int roomNum, GAMEEND_STATE endState);

	InGameRoom* SearchRoom(int roomNum);

	bool DeleteRoomInList(int roomNum);

	ClientSession* GetEnemyClient(ClientSession* player);	

	// �÷��̾� ������ �޾Ƽ� ���� �� ����� ������ ����ü�� ��� �ٽ� ��ȯ
	SCHARACTER SetPlayer(ClientSession* player, SCHARACTER charPacket);

	void SetStopFlag(bool stopFlag) { mStopFlag = stopFlag; }
private:
	std::list<InGameRoom*> mInGameRoomList;

	// ���� ������ ó���� �ʿ��� ��Ŷ�� ��� �� ť�� ���� ó��
	queue<ClientPacket> mInGameBufferQueue;

	int mRoomCount;
	int mLastRoomNum;

	CRITICAL_SECTION mCS;

	bool mStopFlag;
};

