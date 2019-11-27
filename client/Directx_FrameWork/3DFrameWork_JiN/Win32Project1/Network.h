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

	// ���� ����
	void Set_TCPSocket();

	// ���� �õ�
	void Connect(short _portNum);

	void Run();

	//void Threading();

	// ��Ŷ ����
	bool SendPacket(char* _buffer, DWORD _bufferSize);

private:
	WSADATA m_WsaData;
	SOCKET m_ClientSocket;

	//static unsigned int WINAPI RecvThread(LPVOID lpParam);
};

Network* TemplateSingleton<Network>::m_pInstance = NULL;