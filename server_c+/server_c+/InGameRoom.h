#pragma once
#include "stdafx.h"
#include "ClientSession.h"
#include "PlayerInfo.h"

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

private:
	int mRoomNum;
	ClientSession* mPlayer1;
	ClientSession* mPlayer2;
	PlayerInfo mPlayerInfo1;
	PlayerInfo mPlayerInfo2;
	int mMapNum;
};

