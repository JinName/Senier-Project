#pragma once

#include "stdafx.h"

// enum I/O type
enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT,
	IO_DISCONNECT
};

/*
���� ���� ������ ����ü�� ����µ� �̶� WSABUF �� overlapped �� �����Ѵ�.
WSABUF ��WSASend ��WSARecv �Լ��� ���ڷ� ���޵Ǵ� ���ۿ� ���Ǵ� ����ü �̱⿡ ���� �ǰ�
overlapped ����ü ������ �־��ִ°� ���� �Ϸ�� ����� ������ ��� ���� ��� �ȴ�.
*/
struct stOverlapped : public OVERLAPPED
{
	stOverlapped(const ClientSession* client, IOType ioType) : mSessionObject(client), mIOType(ioType)
	{
		// init
		memset(&mWSABuf, 0, sizeof(WSABUF));
	}

	const ClientSession*	mSessionObject;
	IOType					mIOType;
	WSABUF					mWSABuf;
	char					mBuffer[MAX_BUFSIZE];
};

// client session class
// ----- METHOD -----
// OnConnect() : do connect
// IsConnected() : confirm client is connected
// Recv() : recv transfered data by client
// Send() : send to client by server
// Disconnect() : disconnect client
class ClientSession
{
private:
	bool			mIsConnected;	// for get info what client is connected
	SOCKET			mSocket;		// client socket

	SOCKADDR_IN		mClientAddr;	// client address

public:
	ClientSession(SOCKET sock);
	~ClientSession() {}

	bool			OnConnect(SOCKADDR_IN* addr);	// do connect client
	bool			IsConnected(); 					// return client is connected
	bool			Recv();							// recv data by client
	bool			Send(const char* buf, int len);	// send data to client
	bool			DisConnect();					// disconnect
};

