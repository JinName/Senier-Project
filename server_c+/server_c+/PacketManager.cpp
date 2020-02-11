#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"

PacketManager::PacketManager() : mStopFlag(false)
{
	InitializeCriticalSection(&mCS);
}

PacketManager::~PacketManager()
{
	DeleteCriticalSection(&mCS);
}

void PacketManager::Init()
{
	InitializeCriticalSection(&mCS);
}

void PacketManager::Clean()
{
	while (!mBufferQueue.empty())
	{
		mBufferQueue.pop();
	}

	DeleteCriticalSection(&mCS);
}

bool PacketManager::Enqueue(ClientSession* client, char* buffer)
{
	if (buffer == nullptr)
	{
		cout << "Enqueue : buffer is null" << endl;
		return false;
	}

	ClientPacket pack(client, buffer);

	EnterCS();
	mBufferQueue.push(pack);
	LeaveCS();

	return true;
}

bool PacketManager::Dequeue()
{


	return true;
}

void PacketManager::ProcessAllQueue()
{
	// 패킷큐가 전부 처리될 때까지 반복
	while (true)
	{
		if (mStopFlag) break;

		if (!mBufferQueue.empty())
		{
			// 먼저 처리되어야할 패킷을 꺼낸 후 삭제
			EnterCS();

			ClientPacket pack = mBufferQueue.front();
			mBufferQueue.pop();	

			// 패킷 헤드 확인
			PROTOCOL protocol = ParsingPacket(pack);

			// 프로토콜에 따른 패킷 처리
			ProcessPacket(protocol, pack);

			LeaveCS();
		}

		if (mStopFlag) break;
	}
}

PROTOCOL PacketManager::ParsingPacket(ClientPacket pack)
{
	if (pack.mBuffer == nullptr)
	{
		cout << "parsing buffer is nullptr... return PROTOCOL::NONE" << endl;
		return PROTOCOL::NONE;
	}

	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	memcpy(&head, pack.mBuffer, sizeof(SHEAD));

	if (head.mTransferToInGame)
	{
		InGameManager::GetInstance()->Enqueue(pack);

		return PROTOCOL::TRANFERED;
	}

	return (PROTOCOL)head.mCmd;
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
	head.mCmd = (unsigned char)protocol;
	head.mPacketSize = sizeof(SHEAD) + dataBufferSize;

	memcpy(p, (char*)&head, sizeof(SHEAD));
	memcpy(p + sizeof(SHEAD), data, sizeof(data));

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
		break;
	}

	case PROTOCOL::TEST_CHAT:
	{
		SCHAT chat;
		memset(&chat, 0, sizeof(SCHAT));
		memcpy(&chat, pack.mBuffer + sizeof(SHEAD), sizeof(SCHAT));
		cout << "Message From Client : " << chat.buf << endl;
		break;
	}

	case PROTOCOL::MATCH_RQ:
	{
		SMATCH match;
		memset(&match, 0, sizeof(SMATCH));
		memcpy(&match, pack.mBuffer + sizeof(SHEAD), sizeof(SMATCH));
		if (match.mInMatch == true)
		{
			MatchManager::GetInstance()->PushBackClient(pack.mSession);
		}

		break;
	}

	case PROTOCOL::MOVE_RQ:
	{
		// 0. HEAD / DATA 분리
		// 0-1. HEAD
		SHEAD head;
		memset(&head, 0, sizeof(SHEAD));
		memcpy(&head, pack.mBuffer, sizeof(SHEAD));

		// 0-2. DATA
		SCHARACTER playerChar;
		memset(&playerChar, 0, sizeof(SCHARACTER));
		memcpy(&playerChar, pack.mBuffer + sizeof(SHEAD), sizeof(SCHARACTER));

		// 1. MOVE_RQ 를 보낸 클라이언트에 대한 처리
		// 1-1. 서버 내에 각 플레이어 위치연산 후 연산 결과 반환받음
		SCHARACTER newCharData = InGameManager::GetInstance()->SetPlayer(pack.mSession, playerChar);
		
		// 1-2. 클라이언트에 이동 허가 패킷 전송 (MOVE_RP)
		MakeSendPacket(pack.mSession, (char*)&newCharData, sizeof(SCHARACTER), PROTOCOL::MOVE_RP);
		pack.mSession->Send();

		// 2. 다른 클라이언트에서의 처리
		// 2-1. 현재 접속 중인 다른 클라이언트로 MOVE_RQ 를 요청한 플레이어의 상태 브로드캐스팅

		// 플레이어가 입장해 있는 방에서 상대방 Session 을 탐색
		ClientSession* enemySession = InGameManager::GetInstance()->GetEnemyClient(pack.mSession);

		// 값을 찾지 못했을 경우
		if (enemySession == nullptr)
		{
			cout << "enemy session is null.." << endl;
			break;
		}

		// 보내줄 패킷 생성, 전송
		MakeSendPacket(enemySession, (char*)&newCharData, sizeof(SCHARACTER), PROTOCOL::BRCAST_MOVE_RP);
		enemySession->Send();

		break;
	}
	}
}