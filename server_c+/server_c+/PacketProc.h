#pragma once
#include <queue>

#include "stdafx.h"
#include "PacketList.h"

/*
Ŭ������ : PacketProc
��� : WorkerThread ���� ���� ��Ŷ�� �ش� Ŭ������ Queue �� �����ϰ�,
	   �� Ŭ�������� ����� ��Ŷ�� ������ �����忡�� ó���ϸ�,
	   ��Ŷ ������ ���� ó�������� ������.

�帧 : WorkerThread -> PacketQueue -> ProcessPacketThread
*/
class PacketProc
{
public:
	PacketProc();
	~PacketProc();

	// �ش� Ŭ������ �̱���, �Ǵ� �������� ���� �����̱⿡
	// thread-safe �ϰ� ����Ǿ���Ѵ�.
	// ���� CRITICAL_SECTION ���� ó���� �ʼ����̴�.
	bool Enqueue(char* buffer);		// ��Ŷ�� Queue �ȿ� ����
	bool Dequeue();					// ��Ŷ�� Queue ���� ����

	// ��Ŷó��
	void ProcessAllQueue();		// ��� ��Ŷ�� Queue ���� ���� ���� ������ ��Ŷó���� �����

	// ��Ŷ�м�
	bool ParsingPacket();		// HEAD �� �и��Ͽ� ��Ŷ ������ �м� -> � ���������� �����ؾ��ϴ��� �Ǵ�	

	// ���Ǹ� ���� CRITICAL_SECTION �Լ�
	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	// PacketProc Have-A Queue
	// ��Ŷó�� Ŭ���������� ��Ŷ�� ������ ������ ť�� ������.
	// Ŭ���̾�Ʈ���� ���� ��Ŷ�� �׾Ƶΰ�, ������ ProcessPacketThread �� ���� ��Ŷ������ ó���Ѵ�.
	queue<char*> mBufferQueue;

	// for thread-safe
	CRITICAL_SECTION mCS;
};

