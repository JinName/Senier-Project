#pragma once
#include <queue>

#include "stdafx.h"
#include "PacketList.h"
#include "TemplateSingleton.h"
#include "ClientSession.h"


struct ClientPacket
{
	ClientPacket() : mSession(nullptr) { memset(mBuffer, 0, MAX_BUFSIZE); }
	ClientPacket(ClientSession* client, char* buffer, DWORD bufferSize) : mSession(client)
	{
		memset(mBuffer, 0, MAX_BUFSIZE);
		memcpy(mBuffer, buffer, bufferSize);
	}

	ClientSession* mSession;
	char mBuffer[MAX_BUFSIZE];
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

	void Init();
	void Clean();

	// �ش� Ŭ������ �̱���, �Ǵ� �������� ���� �����̱⿡
	// thread-safe �ϰ� ����Ǿ���Ѵ�.
	// ���� CRITICAL_SECTION ���� ó���� �ʼ����̴�.
	bool Enqueue(ClientPacket pack);		// ��Ŷ�� Queue �ȿ� ����
	bool Dequeue(ClientPacket& pack);					// ��Ŷ�� Queue ���� ����

	// ��Ŷó��
	void ProcessAllQueue();		// ��� ��Ŷ�� Queue ���� ���� ���� ������ ��Ŷó���� �����

	// Send ��Ŷ����
	bool MakeSendPacket(ClientSession* client, char* buffer, DWORD dataBufferSize, PROTOCOL protocol);

	// ��Ŷ�м�
	PROTOCOL ParsingPacket(ClientPacket pack);		// HEAD �� �и��Ͽ� ��Ŷ ������ �м� -> � ���������� �����ؾ��ϴ��� �Ǵ�	
	bool CheckAvailablePacket(char* buffer, DWORD dataBufferSize);	// HEAD �� �и��Ͽ� ��Ŷ����� ���������� Ȯ��
	DWORD GetTotalPacketSize(char* buffer, DWORD dataBufferSize);

	// �������ݿ� ���� ��Ŷ ó��
	void ProcessPacket(PROTOCOL protocol, ClientPacket pack);

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }

	void SetStopFlag(bool stopFlag) { mStopFlag = stopFlag; }
private:
	// PacketManager Have-A Queue
	// ��Ŷó�� Ŭ���������� ��Ŷ�� ������ ������ ť�� ������.
	// Ŭ���̾�Ʈ���� ���� ��Ŷ�� �׾Ƶΰ�, ������ ProcessPacketThread �� ���� ��Ŷ������ ó���Ѵ�.
	queue<ClientPacket> mBufferQueue;

	// for thread-safe
	CRITICAL_SECTION mCS;

	// while loop stop flag
	bool mStopFlag;
};

