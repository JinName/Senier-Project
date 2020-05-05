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

	// ���� Ŭ���̾�Ʈ�� �ߺ��ؼ� ��Ī��Ŷ�� �������
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
	// ��� ����Ʈ�� �θ� �̻��� ������ ���
	// ���� ���� ������ �տ������� ��Ī -> ���ӽ���
	while (true)
	{
		if (mStopFlag) break;

		if (mMatchWaitList.size() >= 2)
		{
			// �ڿ� �������� thread-safe �ϰ� �����Ѵ�.
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