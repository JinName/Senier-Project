#include "ClientSession.h"

ClientSession::ClientSession(SOCKET sock) : mIsConnected(false), mSocket(sock)
{
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));	// use memset to initialize sockaddr_in value
}

bool ClientSession::OnConnect()
{
}

bool ClientSession::IsConnected()
{
}

bool ClientSession::Recv()
{
}

bool ClientSession::Send()
{
}

bool ClientSession::DisConnect()
{
}