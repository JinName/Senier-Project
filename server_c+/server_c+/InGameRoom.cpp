#include "InGameRoom.h"

InGameRoom::InGameRoom(ClientSession* _player1, ClientSession* _player2, int _roomNum)
	: mRoomNum(_roomNum), mStopFlag(false)
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
	mGameLogicManager.Init(mClient[0], mClient[1]);
}

void InGameRoom::Clean()
{
	mGameLogicManager.Clean();
}

void InGameRoom::SetPlayer(int _playerIndex, SCHARACTER _charPacket)
{
	//mPlayerInfo[_playerIndex].SetDirection(_charPacket.mDirectionX, 0.0f);

	//if (_charPacket.mLeft)
	//	mPlayerInfo[_playerIndex].Do_Left();
	//else if (_charPacket.mRight)
	//	mPlayerInfo[_playerIndex].Do_Right();

	if (_charPacket.mLeft)
		mGameLogicManager.GetPlayer(_playerIndex)->Do_Left();
	else if (_charPacket.mRight)
		mGameLogicManager.GetPlayer(_playerIndex)->Do_Right();
}

bool InGameRoom::StartGameLogicThread()
{
	DWORD dwThreadId;
	// begin thread
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, GameLogicThread, this, 0, (unsigned int*)&dwThreadId);

	// except error - for create thread
	if (hThread == INVALID_HANDLE_VALUE)
	{
		cout << "Create Thread Fail... " << endl;
		return false;
	}

	// close handle after create
	CloseHandle(hThread);

	return true;
}

unsigned int WINAPI InGameRoom::GameLogicThread(LPVOID lpParam)
{
	InGameRoom* room = (InGameRoom*)lpParam;

	while (true)
	{
		//if (room->GetThreadStopFlag()) break;

		room->GetGameLogicManager()->Update();

		//if (room->GetThreadStopFlag()) break;
	}
	
	return 0;
}