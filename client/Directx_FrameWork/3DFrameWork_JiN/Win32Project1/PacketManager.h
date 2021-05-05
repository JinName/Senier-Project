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

	// 편의를 위한 CRITICAL_SECTION 함수
	void EnterCS() { EnterCriticalSection(&m_CS); }
	void LeaveCS() { LeaveCriticalSection(&m_CS); }

	void SetStopFlag(bool isStop) { m_IsStop = isStop; }
private:
	// PacketManager Have-A Queue
	// 패킷처리 클래스에서는 패킷을 저장할 별도의 큐를 가진다.
	// 클라이언트에서 보낸 패킷을 쌓아두고, 별도의 ProcessPacketThread 를 통해 패킷정보를 처리한다.
	std::queue<char*> m_RecvBufferQueue;

	// for thread-safe
	CRITICAL_SECTION m_CS;

	// flag
	bool m_IsStop;
};

extern PacketManager* g_pPacketManager;
