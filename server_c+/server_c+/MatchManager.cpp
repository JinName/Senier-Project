#include "MatchManager.h"

MatchManager::MatchManager()
{
	InitializeCriticalSection(&mCS);
}

MatchManager::~MatchManager()
{
	DeleteCriticalSection(&mCS);
}

bool MatchManager::Push_Back(ClientSession* client)
{
	if (client == nullptr)
	{
		cout << "MatchManager::Push_Back - client is nullptr.." << endl;
		return false;
	}

	// ���� Ŭ���̾�Ʈ�� �ߺ��ؼ� ��Ī��Ŷ�� �������
	std::list<ClientSession*>::iterator iter = std::find(mMatchWaitList.begin(), mMatchWaitList.end(), client);

	if (iter != mMatchWaitList.end())
	{
		cout << "This Client Already in Match.." << endl;
		return false;
	}

	EnterCS();
	mMatchWaitList.push_back(client);
	LeaveCS();

	return true;
}

void MatchManager::ProcessMatchList()
{
	// ��� ����Ʈ�� �θ� �̻��� ������ ���
	// ���� ���� ������ �տ������� ��Ī -> ���ӽ���
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

		SGAMESTART sGamestart;
		sGamestart.mStart = true;

		bool player1_result = PacketManager::GetInstance()->MakeSendPacket(player1, (char*)&sGamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);
		bool player2_result = PacketManager::GetInstance()->MakeSendPacket(player2, (char*)&sGamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);

		if (player1_result && player2_result)
		{
			player1->Send();
			player2->Send();
		}			
	}
}