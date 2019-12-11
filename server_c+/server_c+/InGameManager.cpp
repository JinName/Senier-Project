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
	mInGameRoomContainer.clear();

	DeleteCriticalSection(&mCS);
}

bool InGameManager::InGame(ClientSession* player1, ClientSession* player2)
{
	if (player1 == nullptr || player2 == nullptr)
	{
		cout << "in Game Fail.." << endl;
		return false;
	}

	SINGAMEROOM* inGameRoom = new SINGAMEROOM(player1, player2, mLastRoomNum + 1);
	inGameRoom->mPlayer1->SetRoomNum(inGameRoom->mRoomNum);
	inGameRoom->mPlayer2->SetRoomNum(inGameRoom->mRoomNum);

	mInGameRoomContainer.push_back(inGameRoom);

	++mLastRoomNum;
	++mRoomCount;

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
	SINGAMEROOM* room = SearchRoom(roomNum);

	// 게임 종료 원인을 패킷으로 전송
	SGAMEEND end;
	end.mGameEndState = endState;

	PacketManager::GetInstance()->MakeSendPacket(room->mPlayer1, (char*)&end, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->mPlayer1->Send();

	PacketManager::GetInstance()->MakeSendPacket(room->mPlayer2, (char*)&end, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->mPlayer2->Send();

	return true;
}

bool InGameManager::OutGame(int roomNum)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	SINGAMEROOM* room = SearchRoom(roomNum);
	room->mPlayer1->SetPlayerIndex(-1);
	room->mPlayer2->SetPlayerIndex(-1);
	room->mPlayer1->SetRoomNum(-1);
	room->mPlayer2->SetRoomNum(-1);
	room->mPlayer1 = nullptr;
	room->mPlayer2 = nullptr;

	delete room;

	--mRoomCount;

	return true;
}

SINGAMEROOM* InGameManager::SearchRoom(int roomNum)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	SINGAMEROOM* room = nullptr;

	std::list<SINGAMEROOM*>::iterator begin_iter = mInGameRoomContainer.begin();
	std::list<SINGAMEROOM*>::iterator end_iter = mInGameRoomContainer.end();

	while (begin_iter != end_iter)
	{
		if ((*begin_iter)->mRoomNum == roomNum)
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

	SINGAMEROOM* room = SearchRoom(player->GetRoomNum());

	if (room == nullptr)
	{
		cout << "room is nullptr.." << endl;
		return NULL;
 	}

	if (room->mPlayer1 == nullptr || room->mPlayer2 == nullptr)
	{
		cout << "enemy client disconnected.." << endl;
		return NULL;
	}

	if (player == room->mPlayer1)
		enemyClient = room->mPlayer2;
	else if (player == room->mPlayer2)
		enemyClient = room->mPlayer1;

	//std::list<SINGAMEROOM*>::iterator begin_iter = mInGameRoomContainer.begin();
	//std::list<SINGAMEROOM*>::iterator end_iter = mInGameRoomContainer.end();

	//while (begin_iter != end_iter)
	//{
	//	if ((*begin_iter)->mPlayer1 == player || (*begin_iter)->mPlayer2 == player)
	//	{
	//		if ((*begin_iter)->mPlayer1 == player)
	//			enemyClient = (*begin_iter)->mPlayer2;
	//		else if ((*begin_iter)->mPlayer2 == player)
	//			enemyClient = (*begin_iter)->mPlayer1;
	//		else // except error
	//		{
	//			cout << "isn't search enemy player.." << endl;
	//			return NULL;
	//		}

	//		break;
	//	}

	//	++begin_iter;
	//}

	return enemyClient;
}