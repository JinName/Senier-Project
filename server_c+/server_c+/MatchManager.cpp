#include "MatchManager.h"
#include "InGameManager.h"

MatchManager::MatchManager() : mStopFlag(false)
{
	InitializeCriticalSection(&mCS);
}

MatchManager::~MatchManager()
{
	DeleteCriticalSection(&mCS);
}

void MatchManager::Init()
{
	InitializeCriticalSection(&mCS);
}

void MatchManager::Clean()
{
	mMatchWaitList.clear();

	DeleteCriticalSection(&mCS);
}

bool MatchManager::PushBackClient(ClientSession* client)
{
	if (client == nullptr)
	{
		cout << "[FAIL] : MatchManager > PushBackClient() > client is nullptr.. > return false" << endl;
		return false;
	}

	// 같은 클라이언트가 중복해서 매칭패킷을 보낼경우
	//std::list<ClientSession*>::iterator iter = std::find(mMatchWaitList.begin(), mMatchWaitList.end(), client);

	if (CheckExistClient(client))
	{
		cout << "[FAIL] : MatchManager > PushBackClient() > client already in match > return false" << endl;
		return false;
	}

	EnterCS();
	mMatchWaitList.push_back(client);
	LeaveCS();

	cout << "[SUCCESS] : MatchManager > PushBackClient()" << endl;

	return true;
}

bool MatchManager::CheckExistClient(ClientSession* client)
{
	if (client == nullptr)
	{
		cout << "[FAIL] : MatchManager > CheckExistClient() > client is nullptr > return false" << endl;
		return false;
	}

	std::list<ClientSession*>::iterator iter = std::find(mMatchWaitList.begin(), mMatchWaitList.end(), client);

	if (iter == mMatchWaitList.end())
	{
		cout << "[INFO] : MatchManager > CheckExistClient() > doesn't exist in match > return false" << endl;
		return false;
	}

	cout << "[INFO] : MatchManager > CheckExistClient() > already exist in match > return true" << endl;
	return true;
}

void MatchManager::ProcessMatchList()
{
	// 대기 리스트에 두명 이상이 존재할 경우
	// 먼저 들어온 순으로 앞에서부터 매칭 -> 게임시작
	while (true)
	{
		if (mStopFlag) break;

		if (mMatchWaitList.size() >= 2)
		{
			// 자원 변경점을 thread-safe 하게 설계한다.
			// start Critical Section
			EnterCS();
			// player 1
			ClientSession* player1 = mMatchWaitList.front();
			mMatchWaitList.pop_front();

			// player 2
			ClientSession* player2 = mMatchWaitList.front();
			mMatchWaitList.pop_front();
			LeaveCS();
			// end Critical Section

			// into GameRoom
			InGameManager::GetInstance()->InGame(player1, player2);			
		}

		if (mStopFlag) break;
	}
}