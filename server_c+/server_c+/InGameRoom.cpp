#include "InGameRoom.h"

InGameRoom::InGameRoom(ClientSession* _player1, ClientSession* _player2, int _roomNum)
	: mRoomNum(_roomNum)
{
	mClient[0] = _player1;
	mClient[1] = _player2;

	for (int i = 0; i < 2; ++i)
	{
		mClient[i]->SetRoomNum(_roomNum);
	}
}

InGameRoom::~InGameRoom()
{
}

void InGameRoom::Init()
{
	// 게임 로직 매니저 초기화
	mGameLogicManager.Init();

	// 캐릭터 정보 초기화(위치 등..)
	for (int i = 0; i < 2; ++i)
	{
		mPlayerInfo[i].Init(i);
	}
}

void InGameRoom::SetPlayer(int _playerIndex, SCHARACTER _charPacket)
{
	mPlayerInfo[_playerIndex].SetDirection(_charPacket.mDirectionX, 0.0f);

	if (_charPacket.mLeft)
		mPlayerInfo[_playerIndex].Do_Left();
	else if (_charPacket.mRight)
		mPlayerInfo[_playerIndex].Do_Right();
}

unsigned int WINAPI InGameRoom::GameLogicThread(LPVOID lpParam)
{
	GameLogicManager* inGameMgr = (GameLogicManager*)lpParam;

	while (true)
	{
		inGameMgr->Update();
	}
	
	return 0;
}