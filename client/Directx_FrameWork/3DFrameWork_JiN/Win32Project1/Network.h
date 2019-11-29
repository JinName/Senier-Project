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
	bool StartRecvThread();
	bool StartPacketProcessThread();

	// get
	SOCKET GetSocket() { return m_ClientSocket; }

	// ��Ŷ ����
	bool SendPacket(PROTOCOL _protocol, char* _data, DWORD _dataSize);



private:
	WSADATA m_WsaData;
	static SOCKET m_ClientSocket;

	static unsigned int WINAPI RecvThread(LPVOID lpParam);
	static unsigned int WINAPI PacketProcessThread(LPVOID lpParam);
};

//Network* TemplateSingleton<Network>::m_pInstance = NULL;