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

	// 소켓 셋팅
	void Set_TCPSocket();

	// 연결 시도
	void Connect(short _portNum);

	void Threading();

	// 패킷 전송
	bool SendPacket(char* _buffer);
};

