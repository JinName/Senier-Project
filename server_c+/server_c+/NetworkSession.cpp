#include "NetworkSession.h"

CNetworkSession::CNetworkSession(void)
{
}


CNetworkSession::~CNetworkSession(void)
{
}


bool CNetworkSession::Begin()
{

	// TCP 家南 积己
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == listenSocket)
	{
		return false;
	}

	if (!TCPBind())
		return false;

	if (listen(listenSocket, 5) == SOCKET_ERROR)
		return false;

	return true;
}

bool CNetworkSession::TCPBind()
{
	SOCKADDR_IN stServerAddr;
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(9000);
	stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int nRet = bind(listenSocket, (SOCKADDR*)& stServerAddr, sizeof(SOCKADDR_IN));

	if (nRet != 0)
	{
		return false;
	}

	return true;
}
