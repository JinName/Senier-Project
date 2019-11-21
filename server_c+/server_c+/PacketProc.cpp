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
	// ��Ŷť�� ���� ó���� ������ �ݺ�
	while (!mBufferQueue.empty())
	{
		// ���� ó���Ǿ���� ��Ŷ�� ���� �� ����
		EnterCS();
		char* buf = mBufferQueue.front();
		mBufferQueue.pop();
		LeaveCS();

		// ��Ŷ�ؼ�����

	}
}