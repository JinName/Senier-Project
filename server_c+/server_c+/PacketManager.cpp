#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"

PacketManager::PacketManager()
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
	// ��Ŷť�� ���� ó���� ������ �ݺ�
	while (!mBufferQueue.empty())
	{
		// ���� ó���Ǿ���� ��Ŷ�� ���� �� ����
		EnterCS();
		ClientPacket pack = mBufferQueue.front();
		mBufferQueue.pop();
		LeaveCS();

		// ��Ŷ ��� Ȯ��
		PROTOCOL protocol = ParsingPacket(pack);

		// �������ݿ� ���� ��Ŷ ó��
		ProcessPacket(protocol, pack);
	}
}

PROTOCOL PacketManager::ParsingPacket(ClientPacket pack)
{
	if (pack.mBuffer == nullptr)
	{
		return PROTOCOL::NONE;
	}

	SHEAD head;

	memcpy(&head, pack.mBuffer, sizeof(SHEAD));

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
	case PROTOCOL::TEST_CHAT:
		SCHAT chat;
		memcpy(&chat, pack.mBuffer + sizeof(SHEAD), sizeof(SCHAT));
		cout << "Message From Client : " << chat.buf << endl;
		break;

	case PROTOCOL::MATCH_RQ:
		SMATCH match;
		memcpy(&match, pack.mBuffer + sizeof(SHEAD), sizeof(SMATCH));
		if (match.mInMatch == true)
		{
			MatchManager::GetInstance()->Push_Back(pack.mSession);
		}
		
		//cout << "Match Request From Client..." << match.mInMatch << endl;
		break;

	case PROTOCOL::MOVE_RQ:
		ClientSession* enemyPlayer = InGameManager::GetInstance()->GetEnemyClient(pack.mSession);
		enemyPlayer->SetSendOverlapped(pack.mBuffer, sizeof(SHEAD) + sizeof(SCHARACTER));
		enemyPlayer->Send();

		break;
	}
}