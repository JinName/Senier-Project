#pragma once
#include "stdafx.h"
#include <process.h>


void RecvThreadPoint(void* param);

class TCPClient
{
private:
	WSADATA m_WsaData;
	SOCKET m_ClientSocket;	

public:
	TCPClient() {}
	~TCPClient() {}

	void Init();
	void Update();
	void Clean();

	// 소켓 셋팅
	void Set_TCPSocket();

	// 연결 시도
	void Connect(short _portNum);

	void Threading();	
};