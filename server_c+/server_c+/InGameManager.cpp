#include "InGameManager.h"

InGameManager::InGameManager() : mRoomCount(0), mLastRoomNum(0)
{
	InitializeCriticalSection(&mCS);
}

InGameManager::~InGameManager()
{
	DeleteCriticalSection(&mCS);
}

void InGameManager::Init()
{
	mRoomCount = 0;
	mLastRoomNum = 0;

	InitializeCriticalSection(&mCS);
}

void InGameManager::Clean()
{
	mInGameRoomList.clear();

	DeleteCriticalSection(&mCS);
}

bool InGameManager::InGame(ClientSession* player1, ClientSession* player2)
{
	if (player1 == nullptr || player2 == nullptr)
	{
		cout << "in Game Fail.." << endl;
		return false;
	}

	// into GameRoom
	InGameRoom* inGameRoom = new InGameRoom(player1, player2, mLastRoomNum + 1);

	// init
	inGameRoom->Init();

	mInGameRoomList.push_back(inGameRoom);

	++mLastRoomNum;
	++mRoomCount;

	// send gamestart packet to client
	// setting player 1
	SGAMESTART player1_Gamestart;
	player1_Gamestart.mStart = true;
	player1_Gamestart.mPlayerIndex = 0; // 1p
	//player1_Gamestart.mPlayerInfo[player1_Gamestart.mPlayerIndex].Init(player1_Gamestart.mPlayerIndex);
	player1->SetPlayerIndex(0);

	// setting player 2
	SGAMESTART player2_Gamestart;
	player2_Gamestart.mStart = true;
	player2_Gamestart.mPlayerIndex = 1; // 2p
	//player1_Gamestart.mPlayerInfo[player1_Gamestart.mPlayerIndex].Init(player1_Gamestart.mPlayerIndex);
	player1->SetPlayerIndex(1);

	bool player1_result = PacketManager::GetInstance()->MakeSendPacket(player1, (char*)&player1_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);
	bool player2_result = PacketManager::GetInstance()->MakeSendPacket(player2, (char*)&player2_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);

	if (player1_result && player2_result)
	{
		// Send Game Start Packet
		player1->Send();
		player2->Send();
	}

	return true;
}

bool InGameManager::GameEnd(int roomNum, GAMEEND_STATE endState)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	// 게임이 끝나는 방 번호로 찾음
	InGameRoom* room = SearchRoom(roomNum);

	// 게임 종료 원인을 패킷으로 전송
	SGAMEEND end;
	end.mGameEndState = endState;

	PacketManager::GetInstance()->MakeSendPacket(room->GetClientSession(0), (char*)&end, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->GetClientSession(0)->Send();

	PacketManager::GetInstance()->MakeSendPacket(room->GetClientSession(1), (char*)&end, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->GetClientSession(1)->Send();

	return true;
}

bool InGameManager::OutGame(int roomNum)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	InGameRoom* room = SearchRoom(roomNum);
	room->GetClientSession(0)->SetPlayerIndex(-1);
	room->GetClientSession(1)->SetPlayerIndex(-1);
	room->GetClientSession(0)->SetRoomNum(-1);
	room->GetClientSession(1)->SetRoomNum(-1);
	room->SetClientSession(0, nullptr);
	room->SetClientSession(1, nullptr);

	delete room;

	--mRoomCount;

	return true;
}

InGameRoom* InGameManager::SearchRoom(int roomNum)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	InGameRoom* room = nullptr;

	std::list<InGameRoom*>::iterator begin_iter = mInGameRoomList.begin();
	std::list<InGameRoom*>::iterator end_iter = mInGameRoomList.end();

	while (begin_iter != end_iter)
	{
		if ((*begin_iter)->GetRoomNum() == roomNum)
		{
			room = *begin_iter;
			break;
		}

		++begin_iter;
	}

	return room;
}

ClientSession* InGameManager::GetEnemyClient(ClientSession* player)
{
	if (player == nullptr)
	{
		cout << "player is nullptr.." << endl;
		return NULL;
	}

	ClientSession* enemyClient = nullptr;

	InGameRoom* room = SearchRoom(player->GetRoomNum());

	if (room == nullptr)
	{
		cout << "room is nullptr.." << endl;
		return NULL;
 	}

	if (room->GetClientSession(0) == nullptr || room->GetClientSession(1) == nullptr)
	{
		cout << "enemy client disconnected.." << endl;
		return NULL;
	}

	if (player == room->GetClientSession(0))
		enemyClient = room->GetClientSession(1);
	else if (player == room->GetClientSession(1))
		enemyClient = room->GetClientSession(0);

	return enemyClient;
}

SCHARACTER InGameManager::SetPlayer(ClientSession* player, SCHARACTER charPacket)
{
	InGameRoom* room = SearchRoom(player->GetRoomNum());

	room->SetPlayer(charPacket.mPlayerIndex, charPacket);

	charPacket.mDirectionX = room->GetPlayerInfo(charPacket.mPlayerIndex).GetDirection().x;
	charPacket.mPosX = room->GetPlayerInfo(charPacket.mPlayerIndex).GetVector3().x;
	charPacket.mPosY = room->GetPlayerInfo(charPacket.mPlayerIndex).GetVector3().y;

	return charPacket;
}