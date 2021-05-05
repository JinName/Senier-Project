#include "InGameRoom.h"

InGameRoom::InGameRoom(ClientSession* _player1, ClientSession* _player2, int _roomNum)
	: m_RoomNum(_roomNum), m_IsStop(false), m_DelayChecker(UPDATE_FPS)
{
	m_Client[0] = _player1;
	m_Client[1] = _player2;

	for (int i = 0; i < 2; ++i)
	{
		m_Client[i]->SetRoomNum(_roomNum);
	}
}

InGameRoom::~InGameRoom()
{
}

void InGameRoom::Init()
{
	// 게임 로직 매니저 초기화
	m_GameLogicManager.Init(m_Client[0], m_Client[1]);
}

void InGameRoom::Clean()
{
	m_GameLogicManager.Clean();
}

void InGameRoom::SetPlayer(int _playerIndex, SCHARACTER _charPacket)
{
	if (_charPacket.m_IsLeft)
		m_GameLogicManager.GetPlayer(_playerIndex)->Do_Left();
	else if (_charPacket.m_IsRight)
		m_GameLogicManager.GetPlayer(_playerIndex)->Do_Right();

	if (_charPacket.m_IsKeyDownSpace)
		m_GameLogicManager.GetPlayer(_playerIndex)->Do_Jump();
}

bool InGameRoom::StartGameLogicThread()
{
	cout << "[INFO] Start In Game Logic Thread!!" << endl;

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

	room->Update();
	
	return 0;
}

void InGameRoom::Update()
{
	while (true)
	{
		if (m_IsStop) break;

		m_DelayChecker.StartCodeRuntimeCheck();

		m_GameLogicManager.Update();

		m_DelayChecker.EndCodeRuntimeCheck();

		m_DelayChecker.DoDelay();

		if (m_IsStop) break;
	}	
}