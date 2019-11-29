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
���� ���� ������ ����ü�� ����µ� �̶� WSABUF �� overlapped �� �����Ѵ�.
WSABUF ��WSASend ��WSARecv �Լ��� ���ڷ� ���޵Ǵ� ���ۿ� ���Ǵ� ����ü �̱⿡ ���� �ǰ�
overlapped ����ü ������ �־��ִ°� ���� �Ϸ�� ����� ������ ��� ���� ��� �ȴ�.
*/
/*
�� ����ü�� ����ϴ� ������ overlapped �ʱ�ȭ�� ����
memset �� ����Ͽ� �ش� ����ü�� �ʱ�ȭ ��ų ���,
�ش� ����ü ���� ��������� ��� ���� �ʱ�ȭ �Ǳ� ������
OVERLAPPED ����ü�� ��ӹ޴� ���º���
�ڽ��� ����� ����ü�� ����� �� �ȿ� OVERLAPPED ������ ������ �ִ� ���°� 
�������� ���鿡�� ���ٰ� �����ȴ�.
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
// ----- ��ɼ��� -----
// �ش� Ŭ������ Ŭ���̾�Ʈ�� ���� �������� CONNECT, DISCONNECT ����� �����ϴ� ���� �ƴϴ�.
// CONNECT, DISCONNECT �� �� ������ Ŭ���̾�Ʈ�� �ش��ϴ� ���������� �����ϱ� ���� Ŭ����.
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
	// const ����� �Լ��� �� �Լ� ������ ��������� ���� �������� �ʴ´ٴ� �ǹ̸� ������,
	// const �� ����� �ش� ��ü���� const �Լ��� �θ� �� �ְԵǰ�,
	// �ٸ� ����Լ����� ������� �� ������ ���ɼ��� ������ �ֱ⶧���� error �� �߻���Ų��.
	bool			IsConnected() const;					// return client is connected
	bool			Recv();									// recv data by client
	bool			Send();									// send data to client

	SOCKET			GetSocket() { return mSocket; }			// return socket

	SOVERLAPPED		GetRecvOverlapped() { return mRecvOverlapped; }
	SOVERLAPPED		GetSendOverlapped() { return mSendOverlapped; }

	char*			GetSendOverlappedBuffer() { return mSendOverlapped.mBuffer; }

	bool			SetSendOverlapped(char* buffer);
};

