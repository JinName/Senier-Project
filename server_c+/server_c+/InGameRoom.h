#pragma once
#include "stdafx.h"
#include "ClientSession.h"
//#include "PlayerInfo.h"
#include "PacketList.h"
#include "GameLogicManager.h"

// class InGameRoom

struct GameRoom
{
	SOCKET sock[2];
};

// ������ �����ϴ� �� ���� ������ ��� Ŭ����
// �ش� ���� ��ȣ(int mRoomNum)
// � Ŭ���̾�Ʈ�� �������ִ���(ClientSession* mPlayer1, mPlayer2)
// �� Ŭ���̾�Ʈ�� ĳ���� ����(PlayerInfo mPlayerInfo1, mPlayerInfo2)
// � �ʿ� �ִ���(int mMapNum)
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
	void				SetClientSession(int _playerIndex, ClientSession* _client)	{ mClient[_playerIndex] = _client; }
	void				SetPlayer(int _playerIndex, SCHARACTER _charPacket);
	void				SetInitComplete(int _playerIndex, bool _initComplete)		{ mInitComplete[_playerIndex] = _initComplete; }

	// get
	ClientSession*		GetClientSession(int _playerIndex)		{ return mClient[_playerIndex]; }
	int					GetRoomNum()							{ return mRoomNum; }
	GameLogicManager*	GetGameLogicManager()					{ return &mGameLogicManager; }
	bool				GetInitComplete(int _playerIndex)		{ return mInitComplete[_playerIndex]; }

	// start logic thread
	bool				StartGameLogicThread();

	// stop flag
	void				SetThreadStopFlag(bool _flag)			{ mStopFlag = _flag; }
	bool				GetThreadStopFlag()						{ return mStopFlag; }

	void				Update();

private:
	int					mRoomNum;
	ClientSession*		mClient[2];
	bool				mInitComplete[2];
	int					mMapNum;

	// in game logic manager
	GameLogicManager	mGameLogicManager;
	bool				mStopFlag;

	// logic thread : room ���� ���� ������ ���
	static unsigned int WINAPI GameLogicThread(LPVOID lpParam);
};

