#include "PacketManager.h"
#include "GameManager.h"

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

		g_pNetwork->SetPlayerIndex(gamestart.mPlayerIndex);

		if (gamestart.mStart == true)
			g_pGameManager->GameStart(gamestart.mPlayerIndex);

		break;
	}

	case PROTOCOL::MOVE_RQ:	
	{
		SCHARACTER enemyChar;
		memset(&enemyChar, 0, sizeof(SCHARACTER));
		memcpy(&enemyChar, recvBuffer + sizeof(SHEAD), sizeof(SCHARACTER));

		g_pGameManager->SetPlayerState(enemyChar);

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
	}

	delete[] recvBuffer;

	return true;
}