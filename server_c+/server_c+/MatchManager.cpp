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

			// setting player 1
			SGAMESTART player1_Gamestart;
			player1_Gamestart.mStart = true;
			player1_Gamestart.mPlayerIndex = 0; // 1p
			player1->SetPlayerIndex(0);

			// setting player 2
			SGAMESTART player2_Gamestart;
			player2_Gamestart.mStart = true;
			player2_Gamestart.mPlayerIndex = 1; // 2p
			player1->SetPlayerIndex(1);

			bool player1_result = PacketManager::GetInstance()->MakeSendPacket(player1, (char*)&player1_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);
			bool player2_result = PacketManager::GetInstance()->MakeSendPacket(player2, (char*)&player2_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);

			if (player1_result && player2_result)
			{
				// 클라이언트 -> InGameRoom 과정 추가 필요
				InGameManager::GetInstance()->InGame(player1, player2);

				// Send Game Start Packet
				player1->Send();
				player2->Send();
			}
		}

		if (mStopFlag) break;
	}
}