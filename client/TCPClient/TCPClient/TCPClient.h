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

	// ���� ����
	void Set_TCPSocket();

	// ���� �õ�
	void Connect(short _portNum);

	void Threading();	
};