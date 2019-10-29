#pragma once

#include <winsock2.h>

class CNetworkSession
{
public:
	CNetworkSession(void);
	~CNetworkSession(void);

	bool Begin();
	bool TCPBind();

	SOCKET GetListenSocket() { return listenSocket; }

private:

	SOCKET listenSocket;
};


