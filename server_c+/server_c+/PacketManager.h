#pragma once
#include <queue>

#include "stdafx.h"
#include "PacketList.h"
#include "TemplateSingleton.h"
#include "ClientSession.h"


struct ClientPacket
{
	ClientPacket() : m_Session(nullptr) { memset(m_Buffer, 0, MAX_BUFSIZE); }
	ClientPacket(ClientSession* client, char* buffer, DWORD bufferSize) : m_Session(client)
	{
		memset(m_Buffer, 0, MAX_BUFSIZE);
		memcpy(m_Buffer, buffer, bufferSize);
	}

	ClientSession* m_Session;
	char m_Buffer[MAX_BUFSIZE];
};

/*
Ŭ������ : PacketManager
��� : workerThread ���� ���� ��Ŷ�� �ش� Ŭ������ Queue �� �����ϰ�,
	   �� Ŭ�������� ����� ��Ŷ�� ������ �����忡�� ó���ϸ�,
	   ��Ŷ ������ ���� ó�������� ������.

�帧 : workerThread -> PacketQueue -> ProcessPacketThread
*/
// PacketManager Is-A Singleton : PacketManager �� Singleton �����̴�.
// 2021-03-10 : singleton -> extern ���·� ����
class PacketManager
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
	bool Dequeue(ClientPacket& pack);		// ��Ŷ�� Queue ���� ����

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
	void EnterCS() { EnterCriticalSection(&m_CS); }
	void LeaveCS() { LeaveCriticalSection(&m_CS); }

	void SetIsStop(bool isStop) { m_IsStop = isStop; }
private:
	// PacketManager Have-A Queue
	// ��Ŷó�� Ŭ���������� ��Ŷ�� ������ ������ ť�� ������.
	// Ŭ���̾�Ʈ���� ���� ��Ŷ�� �׾Ƶΰ�, ������ ProcessPacketThread �� ���� ��Ŷ������ ó���Ѵ�.
	queue<ClientPacket> m_BufferQueue;

	// for thread-safe
	CRITICAL_SECTION m_CS;

	// while loop stop flag
	bool m_IsStop;
};

extern PacketManager* g_pPacketManager;