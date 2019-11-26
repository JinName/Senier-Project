#pragma once
#include <queue>

#include "stdafx.h"
#include "PacketList.h"
#include "TemplateSingleton.h"
#include "ClientSession.h"

struct ClientPacket
{
	ClientPacket(ClientSession* client, char* buffer) : mSession(client), mBuffer(buffer)
	{}

	ClientSession* mSession;
	char* mBuffer;
};

/*
클래스명 : PacketManager
기능 : WorkerThread 에서 받은 패킷을 해당 클래스내 Queue 에 저장하고,
	   이 클래스내에 저장된 패킷을 별도의 스레드에서 처리하며,
	   패킷 정보에 따른 처리로직을 포함함.

흐름 : WorkerThread -> PacketQueue -> ProcessPacketThread
*/
// PacketManager Is-A Singleton : PacketManager 은 Singleton 형태이다.
class PacketManager : public TemplateSingleton<PacketManager>
{
public:
	PacketManager();
	~PacketManager();

	// 해당 클래스는 싱글톤, 또는 전역으로 사용될 목적이기에
	// thread-safe 하게 설계되어야한다.
	// 따라서 CRITICAL_SECTION 등의 처리가 필수적이다.
	bool Enqueue(ClientSession* client, char* buffer);		// 패킷을 Queue 안에 저장
	bool Dequeue();					// 패킷을 Queue 에서 삭제

	// 패킷처리
	void ProcessAllQueue();		// 모든 패킷이 Queue 에서 전부 빠질 때까지 패킷처리를 계속함

	// Send 패킷생성
	bool MakeSendPacket(ClientSession* client, char* buffer, DWORD bufferSize, PROTOCOL protocol);

	// 패킷분석
	PROTOCOL ParsingPacket(ClientPacket pack);		// HEAD 를 분리하여 패킷 정보를 분석 -> 어떤 프로토콜을 실행해야하는지 판단	

	// 프로토콜에 따른 패킷 처리
	void ProcessPacket(PROTOCOL protocol, ClientPacket pack);

	// 편의를 위한 CRITICAL_SECTION 함수
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	// PacketManager Have-A Queue
	// 패킷처리 클래스에서는 패킷을 저장할 별도의 큐를 가진다.
	// 클라이언트에서 보낸 패킷을 쌓아두고, 별도의 ProcessPacketThread 를 통해 패킷정보를 처리한다.
	queue<ClientPacket> mBufferQueue;

	// for thread-safe
	CRITICAL_SECTION mCS;
};

PacketManager* TemplateSingleton<PacketManager>::m_pInstance = NULL;

