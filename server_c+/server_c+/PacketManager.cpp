#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"
#include "GameDBManager.h"

PacketManager* g_pPacketManager = nullptr;

PacketManager::PacketManager() : m_IsStop(false)
{
	InitializeCriticalSection(&m_CS);
}

PacketManager::~PacketManager()
{
	DeleteCriticalSection(&m_CS);
}

void PacketManager::Init()
{
	InitializeCriticalSection(&m_CS);
}

void PacketManager::Clean()
{
	while (!m_BufferQueue.empty())
	{
		m_BufferQueue.pop();
	}

	DeleteCriticalSection(&m_CS);
}

bool PacketManager::Enqueue(ClientPacket pack)
{
	if (pack.m_Session == nullptr)
	{
		cout << "Enqueue : session is null" << endl;
		return false;
	}

	EnterCS();
	m_BufferQueue.push(pack);
	LeaveCS();

	return true;
}

bool PacketManager::Dequeue(ClientPacket& pack)
{
	pack = m_BufferQueue.front();
	m_BufferQueue.pop();

	return true;
}

void PacketManager::ProcessAllQueue()
{
	// 패킷큐가 전부 처리될 때까지 반복
	while (true)
	{
		if (m_IsStop) break;

		if (!m_BufferQueue.empty())
		{
			// 먼저 처리되어야할 패킷을 꺼낸 후 삭제
			EnterCS();

			ClientPacket pack;
			Dequeue(pack);

			LeaveCS();

			// 패킷 헤드 확인
			PROTOCOL protocol = ParsingPacket(pack);

			// 프로토콜에 따른 패킷 처리
			ProcessPacket(protocol, pack);
		}

		if (m_IsStop) break;
	}
}

PROTOCOL PacketManager::ParsingPacket(ClientPacket pack)
{
	if (pack.m_Buffer == nullptr)
	{
		cout << "parsing buffer is nullptr... return PROTOCOL::NONE" << endl;
		return PROTOCOL::NONE;
	}

	// HEAD 확인
	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	memcpy(&head, pack.m_Buffer, sizeof(SHEAD));

	if (head.m_IsTransferToInGame)
	{
		InGameManager::GetInstance()->Enqueue(pack);

		return PROTOCOL::TRANFERED;
	}

	return (PROTOCOL)head.m_Cmd;
}

bool PacketManager::MakeSendPacket(ClientSession* client, char* data, DWORD dataBufferSize, PROTOCOL protocol)
{
	if (client == nullptr)
	{
		cout << "PacketManager::MakeSendPacket - client is nullptr.." << endl;
		return false;
	}

	// make packet
	char* p = client->GetSendOverlappedBuffer();

	// set header
	SHEAD head;
	head.m_Cmd = (unsigned char)protocol;
	head.m_PacketSize = sizeof(SHEAD) + dataBufferSize;
	head.m_IsTransferToInGame = false;

	memcpy(p, (char*)&head, sizeof(SHEAD));
	memcpy(p + sizeof(SHEAD), data, dataBufferSize);

	// set send overlapped
	client->SetSendOverlapped();

	return true;
}

void PacketManager::ProcessPacket(PROTOCOL protocol, ClientPacket pack)
{
	switch (protocol)
	{
	case PROTOCOL::TRANFERED:
	{
		// 다른 패킷처리스레드로 패킷이 전송된 경우
		//cout << "PACKET TRANSFER TO >> InGameManager" << endl;
		break;
	}

	case PROTOCOL::TEST_CHAT:
	{
		SCHAT chat;
		memset(&chat, 0, sizeof(SCHAT));
		memcpy(&chat, pack.m_Buffer + sizeof(SHEAD), sizeof(SCHAT));
		cout << "Message From Client : " << chat.m_Buffer << endl;
		break;
	}

	case PROTOCOL::MATCH_RQ:
	{
		SMATCH match;
		memset(&match, 0, sizeof(SMATCH));
		memcpy(&match, pack.m_Buffer + sizeof(SHEAD), sizeof(SMATCH));
		if (match.m_IsMatch == true)
		{
			MatchManager::GetInstance()->PushBackClient(pack.m_Session);
		}

		break;
	}
	case PROTOCOL::LOGIN_RQ:
	{
		SLOGIN login;
		memset(&login, 0, sizeof(SLOGIN));
		memcpy(&login, pack.m_Buffer + sizeof(SHEAD), sizeof(SLOGIN));

		bool loginResult = g_pGameDBManager->Login(pack.m_Session, login.m_ID, login.m_PW);

		// 성공시 LOGIN_OK, 실패시 LOGIN_DN 전송
		if (loginResult)
		{
			g_pPacketManager->MakeSendPacket(pack.m_Session, NULL, 0, PROTOCOL::LOGIN_OK);
			pack.m_Session->Send();
		}
		else
		{
			g_pPacketManager->MakeSendPacket(pack.m_Session, NULL, 0, PROTOCOL::LOGIN_DN);
			pack.m_Session->Send();
		}

		break;
	}

	case PROTOCOL::MOVE_RQ:
	{
		ClientSession* enemyPlayer = InGameManager::GetInstance()->GetEnemyClient(pack.m_Session);

		if (enemyPlayer == nullptr)
		{
			cout << "enemy player is null.." << endl;
			break;
		}

		enemyPlayer->SetSendOverlapped(pack.m_Buffer, sizeof(SHEAD) + sizeof(SCHARACTER));
		enemyPlayer->Send();

		break;
	}

	case PROTOCOL::CRASH_RQ:
	{
		ClientSession* enemyPlayer = InGameManager::GetInstance()->GetEnemyClient(pack.m_Session);

		if (enemyPlayer == nullptr)
		{
			cout << "enemy player is null.." << endl;
			break;
		}

		enemyPlayer->SetSendOverlapped(pack.m_Buffer, sizeof(SHEAD) + sizeof(SCRASH));
		enemyPlayer->Send();

		break;
	}

	case PROTOCOL::PLAYER_DIE_RQ:
	{
		ClientSession* enemyPlayer = InGameManager::GetInstance()->GetEnemyClient(pack.m_Session);

		if (enemyPlayer == nullptr)
		{
			cout << "enemy player is null.." << endl;
			break;
		}

		SPLAYERDIE sDie;
		memset(&sDie, 0, sizeof(SPLAYERDIE));
		memcpy(&sDie, pack.m_Buffer + sizeof(SHEAD), sizeof(SPLAYERDIE));

		SGAMEEND sGameEnd;
		memset(&sGameEnd, 0, sizeof(SGAMEEND));

		if (sDie.m_PlayerIndex == 0)
			sGameEnd.m_GameEndState = GAMEEND_STATE::P2_WIN;
		else if(sDie.m_PlayerIndex == 1)
			sGameEnd.m_GameEndState = GAMEEND_STATE::P1_WIN;

		g_pPacketManager->MakeSendPacket(enemyPlayer, (char*)&sGameEnd, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
		enemyPlayer->Send();

		break;
	}

	}
}

bool PacketManager::CheckAvailablePacket(char* buffer, DWORD dataBufferSize)
{
	if (dataBufferSize < sizeof(SHEAD))
	{
		cout << "unavailable packet, less than head" << endl;
		return false;
	}

	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	memcpy(&head, buffer, sizeof(SHEAD));

	if (dataBufferSize < head.m_PacketSize)
	{
		cout << "unavailable packet, less size" << endl;
		return false;
	}

	return true;
}

DWORD PacketManager::GetTotalPacketSize(char* buffer, DWORD dataBufferSize)
{
	if (dataBufferSize < sizeof(SHEAD))
	{
		cout << "unavailable packet, less than head" << endl;
		return 0;
	}

	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	memcpy(&head, buffer, sizeof(SHEAD));

	return head.m_PacketSize;
}