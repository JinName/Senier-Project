#include "PacketManager.h"

PacketManager::PacketManager()
{
	InitializeCriticalSection(&mCS);
}

PacketManager::~PacketManager()
{
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

bool PacketManager::MakeSendPacket(ClientSession* client, char* data, DWORD bufferSize, PROTOCOL protocol)
{
	if (client == nullptr)
	{
		cout << "PacketManager::MakeSendPacket - client is nullptr.." << endl;
		return false;
	}

	// make packet
	char* p = client->GetSendOverlapped().mBuffer;

	// set header
	SHEAD head;
	head.mCmd = (unsigned char)protocol;
	head.mDataSize = bufferSize;

	memcpy(p, &head, sizeof(SHEAD));
	memcpy(p + sizeof(SHEAD), data, sizeof(data));

	// set send overlapped
	client->SetSendOverlapped(p);

	return true;
}

void PacketManager::ProcessPacket(PROTOCOL protocol, ClientPacket pack)
{
	switch (protocol)
	{
	case PROTOCOL::TEST_CHAT:
		SCHAT chat;
		memcpy(&chat, pack.mBuffer, sizeof(SCHAT));
		cout << "Message From Client : " << chat.buf << endl;
		break;
	default:
		break;
	}
}