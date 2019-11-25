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
	// 패킷큐가 전부 처리될 때까지 반복
	while (!mBufferQueue.empty())
	{
		// 먼저 처리되어야할 패킷을 꺼낸 후 삭제
		EnterCS();
		ClientPacket pack = mBufferQueue.front();
		mBufferQueue.pop();
		LeaveCS();

		// 패킷 헤드 확인
		PROTOCOL protocol = ParsingPacket(pack);

		// 프로토콜에 따른 패킷 처리
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