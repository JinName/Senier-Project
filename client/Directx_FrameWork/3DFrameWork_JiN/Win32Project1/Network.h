#pragma once
#include "NetworkHeader.h"
#include "PacketList.h"
#include "TemplateSingleton.h"

class Network : public TemplateSingleton<Network>
{
public:
	Network() : m_iPlayerIndex(-1) {}
	~Network() {}

	void Init();
	void Update();
	void Clean();

	// 소켓 셋팅
	void Set_TCPSocket();

	// 연결 시도
	void Connect(short _portNum);

	void Run();

	//void Threading();
	bool StartRecvThread();
	bool StartPacketProcessThread();

	// get
	SOCKET GetSocket() { return m_ClientSocket; }
	int GetPlayerIndex() { return m_iPlayerIndex; }

	// set
	void SetPlayerIndex(int _iPlayerIndex) { m_iPlayerIndex = _iPlayerIndex; }

	// 패킷 전송
	bool SendPacket(PROTOCOL _protocol, char* _data, DWORD _dataSize);

private:
	WSADATA m_WsaData;
	static SOCKET m_ClientSocket;

	static unsigned int WINAPI RecvThread(LPVOID lpParam);
	static unsigned int WINAPI PacketProcessThread(LPVOID lpParam);

	int m_iPlayerIndex;
};