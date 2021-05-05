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
	void EnterCS() { EnterCriticalSection(&m_CS); }
	void LeaveCS() { LeaveCriticalSection(&m_CS); }

	void SetStopFlag(bool isStop) { m_IsStop = isStop; }
private:
	// PacketManager Have-A Queue
	// ��Ŷó�� Ŭ���������� ��Ŷ�� ������ ������ ť�� ������.
	// Ŭ���̾�Ʈ���� ���� ��Ŷ�� �׾Ƶΰ�, ������ ProcessPacketThread �� ���� ��Ŷ������ ó���Ѵ�.
	std::queue<char*> m_RecvBufferQueue;

	// for thread-safe
	CRITICAL_SECTION m_CS;

	// flag
	bool m_IsStop;
};

extern PacketManager* g_pPacketManager;
