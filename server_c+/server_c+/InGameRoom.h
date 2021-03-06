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

// 유저가 대전하는 방 내의 정보를 담는 클래스
// 해당 룸의 번호(int mRoomNum)
// 어떤 클라이언트가 접속해있는지(ClientSession* mPlayer1, mPlayer2)
// 각 클라이언트의 캐릭터 정보(PlayerInfo mPlayerInfo1, mPlayerInfo2)
// 어떤 맵에 있는지(int mMapNum)
// 해당 룸의 충돌처리를 담당할 충돌 매니저(CollisionManager mCollisionMgr)

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

	// logic thread : room 마다 게임 로직을 담당
	static unsigned int WINAPI GameLogicThread(LPVOID lpParam);
};

