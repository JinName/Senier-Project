#pragma once

#include "stdafx.h"

class ClientSession;
class SessionManager;

// enum I/O type
enum class IOTYPE
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
/*
이 구조체를 사용하는 곳에서 overlapped 초기화를 위해
memset 을 사용하여 해당 구조체를 초기화 시킬 경우,
해당 구조체 내의 멤버변수들 모두 값이 초기화 되기 때문에
OVERLAPPED 구조체를 상속받는 형태보다
자신이 사용할 구조체를 만들고 그 안에 OVERLAPPED 변수를 가지고 있는 형태가 
범용적인 측면에서 좋다고 생각된다.
*/
typedef struct sOverlappedSocket : public OVERLAPPED
{
	sOverlappedSocket()
	{
		// init
		memset(mBuffer, 0, MAX_BUFSIZE);
		memset(&mWSABuf, 0, sizeof(WSABUF));
	}
	IOTYPE					mIOType;
	WSABUF					mWSABuf;
	char					mBuffer[MAX_BUFSIZE];
} SOVERLAPPED;

// client session class
// ----- METHOD -----
// OnConnect() : do connect
// IsConnected() : confirm client is connected
// Recv() : recv transfered data by client
// Send() : send to client by server
// Disconnect() : disconnect client
// ----- 기능설명 -----
// 해당 클래스는 클라이언트에 대해 직접적인 CONNECT, DISCONNECT 기능을 수행하는 것이 아니다.
// CONNECT, DISCONNECT 이 후 접속한 클라이언트에 해당하는 소켓정보를 관리하기 위한 클래스.
class ClientSession
{
private:
	bool			mIsConnected;	// for get info what client is connected
	SOCKET			mSocket;		// client socket

	SOCKADDR_IN		mClientAddr;	// client address

	SOVERLAPPED		mRecvOverlapped;// overlapped for recv
	SOVERLAPPED		mSendOverlapped;// overlapped for send

	// game information
	int				mRoomNum;		// -1 : not in game
public:
	ClientSession(SOCKET sock);
	~ClientSession() {}

	bool			OnConnect(SOCKADDR_IN* addr);			// do connect client	
	bool			DisConnect();							// disconnect

	// const function
	// const 선언된 함수는 이 함수 내에서 멤버변수의 값을 수정하지 않는다는 의미를 가지며,
	// const 로 선언된 해당 객체에선 const 함수만 부를 수 있게되고,
	// 다른 멤버함수들은 멤버변수 값 수정의 가능성을 가지고 있기때문에 error 를 발생시킨다.
	bool			IsConnected() const;					// return client is connected
	bool			Recv();									// recv data by client
	bool			Send();									// send data to client

	SOCKET			GetSocket() { return mSocket; }			// return socket

	SOVERLAPPED		GetRecvOverlapped() { return mRecvOverlapped; }
	SOVERLAPPED		GetSendOverlapped() { return mSendOverlapped; }

	char*			GetSendOverlappedBuffer() { return mSendOverlapped.mBuffer; }

	bool			SetSendOverlapped(char* buffer);
};

