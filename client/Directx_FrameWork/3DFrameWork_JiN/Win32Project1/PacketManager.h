#pragma once
#include "NetworkHeader.h"
#include "PacketList.h"
#include "TemplateSingleton.h"
#include "Network.h"
#include <queue>

class PacketManager
{
public:
	PacketManager();
	~PacketManager();

	bool Enqueue(char* recvBuffer);

	void ProcessAllQueue();

	bool ProcessPacket(char* recvBuffer);

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }

	void SetStopFlag(bool stopFlag) { mStopFlag = stopFlag; }
private:
	// PacketManager Have-A Queue
	// ��Ŷó�� Ŭ���������� ��Ŷ�� ������ ������ ť�� ������.
	// Ŭ���̾�Ʈ���� ���� ��Ŷ�� �׾Ƶΰ�, ������ ProcessPacketThread �� ���� ��Ŷ������ ó���Ѵ�.
	std::queue<char*> mRecvBufferQueue;

	// for thread-safe
	CRITICAL_SECTION mCS;

	// flag
	bool mStopFlag;
};

extern PacketManager* g_pPacketManager;
