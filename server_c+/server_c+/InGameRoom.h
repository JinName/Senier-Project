#pragma once
#include "stdafx.h"
#include "ClientSession.h"
#include "PlayerInfo.h"
#include "PacketList.h"

// class InGameRoom

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

