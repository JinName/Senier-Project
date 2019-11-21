#include "PacketProc.h"

PacketProc::PacketProc()
{
	InitializeCriticalSection(&mCS);
}

PacketProc::~PacketProc()
{
	DeleteCriticalSection(&mCS);
}

bool PacketProc::Enqueue(char* buffer)
{
	if (buffer == nullptr)
	{
		cout << "Enqueue : buffer is null" << endl;
		return false;
	}

	EnterCS();
	mBufferQueue.push(buffer);
	LeaveCS();

	return true;
}

bool PacketProc::Dequeue()
{
}

void PacketProc::ProcessAllQueue()
{
	// 패킷큐가 전부 처리될 때까지 반복
	while (!mBufferQueue.empty())
	{
		// 먼저 처리되어야할 패킷을 꺼낸 후 삭제
		EnterCS();
		char* buf = mBufferQueue.front();
		mBufferQueue.pop();
		LeaveCS();

		// 패킷해석시작

	}
}