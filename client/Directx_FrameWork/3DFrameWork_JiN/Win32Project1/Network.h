#pragma once
#include "NetworkHeader.h"
#include "PacketList.h"
#include "TemplateSingleton.h"

class Network : public TemplateSingleton<Network>
{
public:
	void Init();
	void Update();
	void Clean();

	// 소켓 셋팅
	void Set_TCPSocket();

	// 연결 시도
	void Connect(short _portNum);

	void Run();

	//void Threading();

	// 패킷 전송
	bool SendPacket(char* _buffer, DWORD _bufferSize);

private:
	WSADATA m_WsaData;
	SOCKET m_ClientSocket;

	//static unsigned int WINAPI RecvThread(LPVOID lpParam);
};

Network* TemplateSingleton<Network>::m_pInstance = NULL;