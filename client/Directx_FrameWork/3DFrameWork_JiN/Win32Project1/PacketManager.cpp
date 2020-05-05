#include "PacketManager.h"
#include "GameManager.h"
#include "WinSetup.h"

PacketManager* g_pPacketManager = nullptr;

PacketManager::PacketManager() : mStopFlag(false)
{
	InitializeCriticalSection(&mCS);
}

PacketManager::~PacketManager()
{
	while (!mRecvBufferQueue.empty())
	{
		mRecvBufferQueue.pop();
	}
	
	DeleteCriticalSection(&mCS);
}

bool PacketManager::Enqueue(char* recvBuffer)
{
	if (recvBuffer == nullptr)
	{
		cout << "recvBuffer is nullptr.." << endl;
		return false;
	}
	EnterCS();
	mRecvBufferQueue.push(recvBuffer);
	LeaveCS();

	return true;
}

void PacketManager::ProcessAllQueue()
{
	while (true)
	{
		if (mStopFlag) break;

		if (!mRecvBufferQueue.empty())
		{
			EnterCS();
			char* buffer = mRecvBufferQueue.front();
			mRecvBufferQueue.pop();
			LeaveCS();		

			bool result = ProcessPacket(buffer);
		}

		if (mStopFlag) break;
	}
}

bool PacketManager::ProcessPacket(char* recvBuffer)
{
	if (recvBuffer == nullptr)
	{
		return false;
	}

	SHEAD head;
	memcpy(&head, recvBuffer, sizeof(SHEAD));
	PROTOCOL protocol = (PROTOCOL)head.mCmd;

	switch (protocol)
	{
	case PROTOCOL::GAMESTART_CM:	
	{
		SGAMESTART gamestart;
		memset(&gamestart, 0, sizeof(SGAMESTART));
		memcpy(&gamestart, recvBuffer + sizeof(SHEAD), sizeof(SGAMESTART));
		int datasize = sizeof(recvBuffer);

		g_pNetwork->SetPlayerIndex(gamestart.mPlayerIndex);

		if (gamestart.mStart == true)
		{
			g_pGameManager->GameStart(gamestart.mPlayerIndex);
			g_pGameManager->SetStartPosition(gamestart.mStartPosition[0], gamestart.mStartPosition[1]);
		}

		break;
	}

	case PROTOCOL::MOVE_RQ:	
	{
		SCHARACTER player;
		memset(&player, 0, sizeof(SCHARACTER));
		memcpy(&player, recvBuffer + sizeof(SHEAD), sizeof(SCHARACTER));

		g_pGameManager->SetPlayerState(player);

		break;
	}

	case PROTOCOL::BRCAST_MOVE_RP:
	{
		SCHARACTER otherPlayer;
		memset(&otherPlayer, 0, sizeof(SCHARACTER));
		memcpy(&otherPlayer, recvBuffer + sizeof(SHEAD), sizeof(SCHARACTER));

		g_pGameManager->SetPlayerState(otherPlayer);

		break;
	}

	case PROTOCOL::GAMEEND_CM:
	{
		SGAMEEND end;
		memset(&end, 0, sizeof(SGAMEEND));
		memcpy(&end, recvBuffer + sizeof(SHEAD), sizeof(SGAMEEND));

		g_pGameManager->GameOver();

		break;
	}

	case PROTOCOL::UPDATE_NF:
	{
		SCHARACTER player;
		memset(&player, 0, sizeof(SCHARACTER));
		memcpy(&player, recvBuffer + sizeof(SHEAD), sizeof(SCHARACTER));
		
		g_pGameManager->SetPlayerPosition(player);

		break;
	}

	case PROTOCOL::LOGIN_OK:
	{
		g_pWinSetup->Init();

		break;
	}

	}

	delete[] recvBuffer;

	return true;
}