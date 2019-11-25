#include "PacketProc.h"

PacketProc::PacketProc()
{
	InitializeCriticalSection(&mCS);
}

PacketProc::~PacketProc()
{
	DeleteCriticalSection(&mCS);
}

bool PacketProc::Enqueue(ClientSession* client, char* buffer)
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

bool PacketProc::Dequeue()
{
	return true;
}

void PacketProc::ProcessAllQueue()
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

PROTOCOL PacketProc::ParsingPacket(ClientPacket pack)
{
	if (pack.mBuffer == nullptr)
	{
		return PROTOCOL::NONE;
	}

	SHEAD head;

	memcpy(&head, pack.mBuffer, sizeof(SHEAD));

	return (PROTOCOL)head.mCmd;
}

void PacketProc::ProcessPacket(PROTOCOL protocol, ClientPacket pack)
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