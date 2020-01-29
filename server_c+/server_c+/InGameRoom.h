#pragma once
#include "stdafx.h"
#include "ClientSession.h"
#include "PlayerInfo.h"
#include "PacketList.h"

// class InGameRoom

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
	void			Init();

	// set
	void			SetClientSession(int _playerIndex, ClientSession* _client) { mClient[_playerIndex] = _client; }
	void			SetPlayer(int _playerIndex, SCHARACTER _charPacket);

	// get
	ClientSession*	GetClientSession(int _playerIndex)		{ return mClient[_playerIndex]; }
	PlayerInfo		GetPlayerInfo(int _playerIndex)		{ return mPlayerInfo[_playerIndex]; }
	int				GetRoomNum()							{ return mRoomNum; }

private:
	int				mRoomNum;
	ClientSession*	mClient[2];
	PlayerInfo		mPlayerInfo[2];
	int				mMapNum;
};

