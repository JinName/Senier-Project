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

	// �÷��̾� ������ �޾Ƽ� ���� �� ����� ������ ����ü�� ��� �ٽ� ��ȯ
	SCHARACTER SetPlayer(ClientSession* player, SCHARACTER charPacket);

	void SetIsStop(bool isStop) { m_IsStop = isStop; }

	std::list<InGameRoom*>* GetRoomListPtr() { return &m_InGameRoomList; }
private:
	std::list<InGameRoom*> m_InGameRoomList;

	std::vector<std::list<InGameRoom*>> m_ListVector;

	// ���� ������ ó���� �ʿ��� ��Ŷ�� ��� �� ť�� ���� ó��
	queue<ClientPacket> m_InGameBufferQueue;

	int m_RoomCount;
	int m_LastRoomNum;

	CRITICAL_SECTION m_CS;

	bool m_IsStop;
};

