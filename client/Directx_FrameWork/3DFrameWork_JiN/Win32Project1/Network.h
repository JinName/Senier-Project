#pragma once
#include "NetworkHeader.h"
#include "PacketList.h"

class Network
{
private:
	WSADATA m_WsaData;
	SOCKET m_ClientSocket;

public:
	void Init();
	void Update();
	void Clean();

	// ���� ����
	void Set_TCPSocket();

	// ���� �õ�
	void Connect(short _portNum);

	void Threading();

	// ��Ŷ ����
	bool SendPacket(char* _buffer);
};

