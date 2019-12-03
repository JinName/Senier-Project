#include "PacketManager.h"
#include "GameManager.h"

PacketManager::PacketManager()
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
		if (!mRecvBufferQueue.empty())
		{
			EnterCS();
			char* buffer = mRecvBufferQueue.front();
			mRecvBufferQueue.pop();
			LeaveCS();

			bool result = ProcessPacket(buffer);
		}
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
		SGAMESTART gamestart;
		memcpy(&gamestart, recvBuffer + sizeof(SHEAD), sizeof(SGAMESTART));
		
		Network::GetInstance()->SetPlayerIndex(gamestart.mPlayerIndex);

		if (gamestart.mStart == true)
			g_pGameManager->GameStart(gamestart.mPlayerIndex);			

		break;

	case PROTOCOL::MOVE_RQ:
		SCHARACTER enemyChar;
		memcpy(&enemyChar, recvBuffer + sizeof(SHEAD), sizeof(SCHARACTER));

		g_pGameManager->SetPlayerState(enemyChar);

		break;	
	}

	delete[] recvBuffer;

	return true;
}

void PacketManager::Init()
{
}

void PacketManager::Clean()
{
	while (!mRecvBufferQueue.empty())
	{
		mRecvBufferQueue.pop();
	}

	DeleteCriticalSection(&mCS);

	m_bClean = true;
}