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
소켓 버퍼 정보를 구조체로 만드는데 이때 WSABUF 와 overlapped 를 포함한다.
WSABUF 는WSASend 나WSARecv 함수의 인자로 전달되는 버퍼에 사용되는 구조체 이기에 포함 되고
overlapped 구조체 변수를 넣어주는건 현재 완료된 입출력 정보를 얻어 낼때 사용 된다.
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

