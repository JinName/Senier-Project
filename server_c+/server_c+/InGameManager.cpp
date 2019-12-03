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

	SINGAMEROOM inGameRoom;
	inGameRoom.mPlayer1 = player1;
	inGameRoom.mPlayer2 = player2;
	inGameRoom.mRoomNum = mLastRoomNum + 1;
	//inGameRoom.mPlayerInfo[0].Init(0); // 1p
	//inGameRoom.mPlayerInfo[1].Init(1); // 2p

	mInGameRoomContainer.push_back(inGameRoom);

	++mLastRoomNum;
	++mRoomCount;

	return true;
}

bool InGameManager::OutGame(int roomNum)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	//std::list<SINGAMEROOM>::iterator begin_iter = mInGameRoomList.begin();
	//std::list<SINGAMEROOM>::iterator end_iter = mInGameRoomList.end();

	//while (begin_iter != end_iter)
	//{
	//	if (begin_iter->mRoomNum == roomNum)
	//	{
	//		begin_iter = mInGameRoomList.erase(begin_iter);
	//		break;
	//	}
	//	else
	//	{
	//		++begin_iter;
	//	}	

	//	// except error
	//	if (begin_iter == end_iter)
	//	{
	//		cout << "wrong room number.." << endl;
	//		return false;
	//	}
	//}

	--mRoomCount;

	return true;
}

ClientSession* InGameManager::GetEnemyClient(ClientSession* player)
{
	if (player == nullptr)
	{
		cout << "player is nullptr.." << endl;
		return NULL;
	}

	ClientSession* enemyClient = nullptr;

	//SINGAMEROOM inGameRoom;

	//for (int i = 0; i < mInGameRoomContainer.size(); ++i)
	//{
	//	if (mInGameRoomContainer[i].mRoomNum == player->GetRoomNum())
	//	{
	//		//return mInGameRoomContainer[i].
	//	}
	//}

	std::list<SINGAMEROOM>::iterator begin_iter = mInGameRoomContainer.begin();
	std::list<SINGAMEROOM>::iterator end_iter = mInGameRoomContainer.end();

	while (begin_iter != end_iter)
	{
		if (begin_iter->mPlayer1 == player || begin_iter->mPlayer2 == player)
		{
			if (begin_iter->mPlayer1 == player)
				enemyClient = begin_iter->mPlayer2;
			else if (begin_iter->mPlayer2 == player)
				enemyClient = begin_iter->mPlayer1;
			else // except error
			{
				cout << "isn't search enemy player.." << endl;
				return NULL;
			}

			break;
		}

		++begin_iter;
	}

	return enemyClient;
}