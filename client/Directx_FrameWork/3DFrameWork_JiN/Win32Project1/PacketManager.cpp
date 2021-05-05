#include "PacketManager.h"
#include "GameManager.h"
#include "WinSetup.h"

PacketManager* g_pPacketManager = nullptr;

PacketManager::PacketManager() : m_IsStop(false)
{
	InitializeCriticalSection(&m_CS);
}

PacketManager::~PacketManager()
{
	while (!m_RecvBufferQueue.empty())
	{
		m_RecvBufferQueue.pop();
	}
	
	DeleteCriticalSection(&m_CS);
}

bool PacketManager::Enqueue(char* recvBuffer)
{
	if (recvBuffer == nullptr)
	{
		cout << "recvBuffer is nullptr.." << endl;
		return false;
	}
	EnterCS();
	m_RecvBufferQueue.push(recvBuffer);
	LeaveCS();

	return true;
}

void PacketManager::ProcessAllQueue()
{
	while (true)
	{
		if (m_IsStop) break;

		if (!m_RecvBufferQueue.empty())
		{
			EnterCS();
			char* buffer = m_RecvBufferQueue.front();
			m_RecvBufferQueue.pop();
			LeaveCS();		

			bool result = ProcessPacket(buffer);
		}

		if (m_IsStop) break;
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
	PROTOCOL protocol = (PROTOCOL)head.m_Cmd;

	switch (protocol)
	{
	case PROTOCOL::GAMESTART_CM:	
	{
		SGAMESTART gamestart;
		memset(&gamestart, 0, sizeof(SGAMESTART));
		memcpy(&gamestart, recvBuffer + sizeof(SHEAD), sizeof(SGAMESTART));
		int datasize = sizeof(recvBuffer);

		g_pNetwork->SetPlayerIndex(gamestart.m_PlayerIndex);

		if (gamestart.m_IsStart == true)
		{
			g_pGameManager->GameStart(gamestart.m_PlayerIndex);
			g_pGameManager->SetStartPosition(gamestart.m_StartPosition[0], gamestart.m_StartPosition[1]);
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
		SGAMEEND m_IsEnd;
		memset(&m_IsEnd, 0, sizeof(SGAMEEND));
		memcpy(&m_IsEnd, recvBuffer + sizeof(SHEAD), sizeof(SGAMEEND));

		if (g_pNetwork->GetPlayerIndex() == 0)
		{
			if (m_IsEnd.m_GameEndState == GAMEEND_STATE::P1_WIN)
				g_pGameManager->GameClear();
			else
				g_pGameManager->GameOver();
		}
		else if (g_pNetwork->GetPlayerIndex() == 1)
		{
			if (m_IsEnd.m_GameEndState == GAMEEND_STATE::P2_WIN)
				g_pGameManager->GameClear();
			else
				g_pGameManager->GameOver();
		}	

		break;
	}

	case PROTOCOL::UPDATE_NF:
	{
		SINGAMEINFO info;
		memset(&info, 0, sizeof(SINGAMEINFO));
		memcpy(&info, recvBuffer + sizeof(SHEAD), sizeof(SINGAMEINFO));
		
		g_pGameManager->SetPlayerPosition(info.m_CharInfo[0]);
		g_pGameManager->SetPlayerPosition(info.m_CharInfo[1]);

		break;
	}

	case PROTOCOL::LOGIN_OK:
	{
		g_pWinSetup->Init();

		break;
	}

	case PROTOCOL::CRASH_RQ:
	{
		SCRASH crashPlayer;
		memset(&crashPlayer, 0, sizeof(SCRASH));
		memcpy(&crashPlayer, recvBuffer + sizeof(SHEAD), sizeof(SCRASH));

		g_pGameManager->SetPlayerState(crashPlayer);

		break;
	}

	}

	delete[] recvBuffer;

	return true;
}