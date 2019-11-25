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
Ŭ������ : PacketProc
��� : WorkerThread ���� ���� ��Ŷ�� �ش� Ŭ������ Queue �� �����ϰ�,
	   �� Ŭ�������� ����� ��Ŷ�� ������ �����忡�� ó���ϸ�,
	   ��Ŷ ������ ���� ó�������� ������.

�帧 : WorkerThread -> PacketQueue -> ProcessPacketThread
*/
// PacketProc Is-A Singleton : PacketProc �� Singleton �����̴�.
class PacketProc : public TemplateSingleton<PacketProc>
{
public:
	PacketProc();
	~PacketProc();

	// �ش� Ŭ������ �̱���, �Ǵ� �������� ���� �����̱⿡
	// thread-safe �ϰ� ����Ǿ���Ѵ�.
	// ���� CRITICAL_SECTION ���� ó���� �ʼ����̴�.
	bool Enqueue(ClientSession* client, char* buffer);		// ��Ŷ�� Queue �ȿ� ����
	bool Dequeue();					// ��Ŷ�� Queue ���� ����

	// ��Ŷó��
	void ProcessAllQueue();		// ��� ��Ŷ�� Queue ���� ���� ���� ������ ��Ŷó���� �����

	// ��Ŷ�м�
	PROTOCOL ParsingPacket(ClientPacket pack);		// HEAD �� �и��Ͽ� ��Ŷ ������ �м� -> � ���������� �����ؾ��ϴ��� �Ǵ�	

	// �������ݿ� ���� ��Ŷ ó��
	void ProcessPacket(PROTOCOL protocol, ClientPacket pack);

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	// PacketProc Have-A Queue
	// ��Ŷó�� Ŭ���������� ��Ŷ�� ������ ������ ť�� ������.
	// Ŭ���̾�Ʈ���� ���� ��Ŷ�� �׾Ƶΰ�, ������ ProcessPacketThread �� ���� ��Ŷ������ ó���Ѵ�.
	queue<ClientPacket> mBufferQueue;

	// for thread-safe
	CRITICAL_SECTION mCS;
};

PacketProc* TemplateSingleton<PacketProc>::m_pInstance = NULL;

