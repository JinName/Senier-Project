#pragma once

#include "stdafx.h"
#include "CircularBuffer.h"

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
		memset(m_Buffer, 0, MAX_BUFSIZE);
		memset(&m_WSABuf, 0, sizeof(WSABUF));
	}
	IOTYPE					m_IOType;
	WSABUF					m_WSABuf;
	char					m_Buffer[MAX_BUFSIZE];
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
	bool			m_IsConnected;	// for get info what client is connected
	SOCKET			m_Socket;		// client socket

	SOCKADDR_IN		m_ClientAddr;	// client address

	SOVERLAPPED		m_RecvOverlapped;// overlapped for recv
	SOVERLAPPED		m_SendOverlapped;// overlapped for send

	// game information
	int				m_RoomNum;		// -1 : not in game
	int				m_PlayerIndex;	// -1 : not in game

	char			m_ID[MAX_ID_LEN];
	bool			m_IsLogin;
	
	// circular buffer
	CircularBuffer	m_RingBuffer;

public:
	ClientSession(SOCKET socket);
	~ClientSession() {}

	bool			OnConnect(SOCKADDR_IN* address);			// do connect client	
	bool			DisConnect();							// disconnect

	// const function
	// const ����� �Լ��� �� �Լ� ������ ��������� ���� �������� �ʴ´ٴ� �ǹ̸� ������,
	// const �� ����� �ش� ��ü���� const �Լ��� �θ� �� �ְԵǰ�,
	// �ٸ� ����Լ����� ������� �� ������ ���ɼ��� ������ �ֱ⶧���� error �� �߻���Ų��.
	bool			IsConnected() const;					// return client is connected
	bool			Recv();									// recv data by client
	bool			Send();									// send data to client

	// get ///////////////////////
	SOCKET			GetSocket()			{ return m_Socket; } // return socket
	int				GetRoomNum()		{ return m_RoomNum; }

	SOVERLAPPED		GetRecvOverlapped() { return m_RecvOverlapped; }
	SOVERLAPPED		GetSendOverlapped() { return m_SendOverlapped; }

	char*			GetSendOverlappedBuffer() { return m_SendOverlapped.m_Buffer; }

	// set ///////////////////////
	void			SetRoomNum(int roomNum)			{ m_RoomNum = roomNum; }
	void			SetPlayerIndex(int playerIndex)	{ m_PlayerIndex = playerIndex; }

	bool			SetSendOverlapped();
	bool			SetSendOverlapped(char* buffer, int bufferSize);

	// id
	void			SetID(char* id) { strcpy(m_ID, id); }
	char*			GetID() { return m_ID; }
	void			CleanID() { memset(m_ID, 0, MAX_ID_LEN); }

	// login
	void			SetIsLogin(bool isLogin) { m_IsLogin = isLogin; }
	bool			GetIsLogin() { return m_IsLogin; }

	// ring buffer
	void			CompleteRecv(DWORD dataSize);
	bool			PopBuffer(char* outBuffer);
};

