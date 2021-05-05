#pragma once
#include "stdafx.h"
#include "ClientSession.h"
//#include "PlayerInfo.h"
#include "PacketList.h"
#include "GameLogicManager.h"
#include "FpsDelayChecker.h"

#define UPDATE_FPS 120

// class InGameRoom

struct GameRoom
{
	SOCKET socket[2];
};

// ������ �����ϴ� �� ���� ������ ��� Ŭ����
// �ش� ���� ��ȣ(int m_RoomNum)
// � Ŭ���̾�Ʈ�� �������ִ���(ClientSession* mPlayer1, mPlayer2)
// �� Ŭ���̾�Ʈ�� ĳ���� ����(PlayerInfo mPlayerInfo1, mPlayerInfo2)
// � �ʿ� �ִ���(int m_MapNum)
// �ش� ���� �浹ó���� ����� �浹 �Ŵ���(CollisionManager mCollisionMgr)

class InGameRoom
{
public:
	InGameRoom(ClientSession* player1, ClientSession* player2, int roomNum);
	~InGameRoom();

	// init
	void				Init();
	// clean
	void				Clean();

	// set
	void				SetClientSession(int _playerIndex, ClientSession* client)	{ m_Client[_playerIndex] = client; }
	void				SetPlayer(int _playerIndex, SCHARACTER _charPacket);
	void				SetInitComplete(int _playerIndex, bool _initComplete)		{ m_InitComplete[_playerIndex] = _initComplete; }

	// get
	ClientSession*		GetClientSession(int _playerIndex)		{ return m_Client[_playerIndex]; }
	int					GetRoomNum()							{ return m_RoomNum; }
	GameLogicManager*	GetGameLogicManager()					{ return &m_GameLogicManager; }
	bool				GetInitComplete(int _playerIndex)		{ return m_InitComplete[_playerIndex]; }

	// start logic thread
	bool				StartGameLogicThread();

	// stop flag
	void				SetThreadStopFlag(bool _flag)			{ m_IsStop = _flag; }
	bool				GetThreadStopFlag()						{ return m_IsStop; }

	void				Update();

private:
	int					m_RoomNum;
	ClientSession*		m_Client[2];
	bool				m_InitComplete[2];
	int					m_MapNum;

	// in game logic manager
	GameLogicManager	m_GameLogicManager;
	bool				m_IsStop;

	FpsDelayChecker		m_DelayChecker;

	// logic thread : room ���� ���� ������ ���
	static unsigned int WINAPI GameLogicThread(LPVOID lpParam);
};

