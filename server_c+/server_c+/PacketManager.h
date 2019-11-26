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
Ŭ������ : PacketManager
��� : WorkerThread ���� ���� ��Ŷ�� �ش� Ŭ������ Queue �� �����ϰ�,
	   �� Ŭ�������� ����� ��Ŷ�� ������ �����忡�� ó���ϸ�,
	   ��Ŷ ������ ���� ó�������� ������.

�帧 : WorkerThread -> PacketQueue -> ProcessPacketThread
*/
// PacketManager Is-A Singleton : PacketManager �� Singleton �����̴�.
class PacketManager : public TemplateSingleton<PacketManager>
{
public:
	PacketManager();
	~PacketManager();

	// �ش� Ŭ������ �̱���, �Ǵ� �������� ���� �����̱⿡
	// thread-safe �ϰ� ����Ǿ���Ѵ�.
	// ���� CRITICAL_SECTION ���� ó���� �ʼ����̴�.
	bool Enqueue(ClientSession* client, char* buffer);		// ��Ŷ�� Queue �ȿ� ����
	bool Dequeue();					// ��Ŷ�� Queue ���� ����

	// ��Ŷó��
	void ProcessAllQueue();		// ��� ��Ŷ�� Queue ���� ���� ���� ������ ��Ŷó���� �����

	// Send ��Ŷ����
	bool MakeSendPacket(ClientSession* client, char* buffer, DWORD bufferSize, PROTOCOL protocol);

	// ��Ŷ�м�
	PROTOCOL ParsingPacket(ClientPacket pack);		// HEAD �� �и��Ͽ� ��Ŷ ������ �м� -> � ���������� �����ؾ��ϴ��� �Ǵ�	

	// �������ݿ� ���� ��Ŷ ó��
	void ProcessPacket(PROTOCOL protocol, ClientPacket pack);

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	// PacketManager Have-A Queue
	// ��Ŷó�� Ŭ���������� ��Ŷ�� ������ ������ ť�� ������.
	// Ŭ���̾�Ʈ���� ���� ��Ŷ�� �׾Ƶΰ�, ������ ProcessPacketThread �� ���� ��Ŷ������ ó���Ѵ�.
	queue<ClientPacket> mBufferQueue;

	// for thread-safe
	CRITICAL_SECTION mCS;
};

PacketManager* TemplateSingleton<PacketManager>::m_pInstance = NULL;

