#include "MatchManager.h"
#include "InGameManager.h"

MatchManager::MatchManager() : m_IsStop(false)
{
	InitializeCriticalSection(&m_CS);
}

MatchManager::~MatchManager()
{
	DeleteCriticalSection(&m_CS);
}

void MatchManager::Init()
{
	InitializeCriticalSection(&m_CS);
}

void MatchManager::Clean()
{
	m_MatchWaitList.clear();

	DeleteCriticalSection(&m_CS);
}

bool MatchManager::PushBackClient(ClientSession* client)
{
	if (client == nullptr)
	{
		cout << "[FAIL] : MatchManager > PushBackClient() > client is nullptr.. > return false" << endl;
		return false;
	}

	if (CheckExistClient(client))
	{
		cout << "[FAIL] : MatchManager > PushBackClient() > client already in match > return false" << endl;
		return false;
	}

	EnterCS();
	m_MatchWaitList.push_back(client);
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

	std::list<ClientSession*>::iterator iter = std::find(m_MatchWaitList.begin(), m_MatchWaitList.m_IsEnd(), client);

	if (iter == m_MatchWaitList.m_IsEnd())
	{
		cout << "[INFO] : MatchManager > CheckExistClient() > doesn't exist in match > return false" << endl;
		return false;
	}

	cout << "[INFO] : MatchManager > CheckExistClient() > already exist in match > return true" << endl;
	return true;
}

bool MatchManager::DeleteClient(ClientSession* client)
{
	std::list<ClientSession*>::iterator iter = std::find(m_MatchWaitList.begin(), m_MatchWaitList.m_IsEnd(), client);

	m_MatchWaitList.erase(iter);

	cout << "[INFO] : MatchManager > DeleteClient() > delete clientsession from wait list.." << endl;

	return false;
}

void MatchManager::ProcessMatchList()
{
	// 대기 리스트에 두명 이상이 존재할 경우
	// 먼저 들어온 순으로 앞에서부터 매칭 -> 게임시작
	while (true)
	{
		if (m_IsStop) break;

		if (m_MatchWaitList.size() >= 2)
		{
			// 자원 변경점을 thread-safe 하게 설계한다.
			// start Critical Section
			EnterCS();
			// player 1
			ClientSession* player1 = m_MatchWaitList.front();
			m_MatchWaitList.pop_front();

			// player 2
			ClientSession* player2 = m_MatchWaitList.front();
			m_MatchWaitList.pop_front();
			LeaveCS();
			// m_IsEnd Critical Section

			// into GameRoom
			InGameManager::GetInstance()->InGame(player1, player2);			
		}

		if (m_IsStop) break;
	}
}