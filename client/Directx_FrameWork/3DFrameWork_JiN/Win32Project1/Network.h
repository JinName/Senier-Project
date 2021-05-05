#pragma once
#include "NetworkHeader.h"
#include "PacketList.h"
#include "TemplateSingleton.h"

class Network
{
public:
	Network();
	~Network();

	void Init();
	void Update();
	void Clean();

	// ���� ����
	void Set_TCPSocket();

	// ���� �õ�
	void Connect(short portNum);

	void Run();

	//void Threading();
	bool StartRecvThread();
	bool StartPacketProcessThread();

	// get
	SOCKET GetSocket() { return m_ClientSocket; }
	int GetPlayerIndex() { return m_PlayerIndex; }

	// set
	void SetPlayerIndex(int playerIndex) { m_PlayerIndex = playerIndex; }

	// ��Ŷ ����
	bool SendPacket(PROTOCOL protocol, char* data, DWORD dataSize, bool inGame);

private:
	WSADATA m_WsaData;
	static SOCKET m_ClientSocket;

	static unsigned int WINAPI recvThread(LPVOID lpParam);
	static unsigned int WINAPI packetProcessThread(LPVOID lpParam);

	int m_PlayerIndex;
};

extern Network* g_pNetwork;