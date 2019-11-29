#include "ClientSession.h"
#include "IOCPManager.h"
#include "SessionManager.h"

ClientSession::ClientSession(SOCKET sock) : mIsConnected(false), mSocket(sock), mRoomNum(-1)
{
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));	// use memset to initialize sockaddr_in value
	memset(&mRecvOverlapped, 0, sizeof(SOVERLAPPED));
	memset(&mSendOverlapped, 0, sizeof(SOVERLAPPED));
}

/*
�Լ��� : OnConnect()

���ڰ� :
1) SOCKADDR_IN* addr : Ŭ���̾�Ʈ �ּ�

��� : ���޹��� Ŭ���̾�Ʈ �ּҸ� ����,
	   ���޹��� �ּҿ� ���� Ŭ���̾�Ʈ�� �����ϴ��� Ȯ��.
*/
bool ClientSession::OnConnect(SOCKADDR_IN* addr)
{
	// setting socket I/O mode - not 0 : nonblocking mode / 0 : blocking mode
	u_long arg = 1;
	ioctlsocket(mSocket, FIONBIO, &arg);

	// setting socket option
	int opt = 1;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)& opt, sizeof(int));

	// setting recv buf option - (SOCKET, SQL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(int))
	// SQL_SOCKET option
	// SO_RCVBUF, SO_SNDBUF - setting recv or send bufsize
	opt = 0;
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)& opt, sizeof(int)))
	{
		cout << "SO_RCVBUF change error: " << GetLastError() << endl;
		return false;
	}

	// connect socket to I/O completion port
	// ����° ������ CompletionKey �� ���� ���ϴ� ������ �ѱ���,
	// GetQueuedCompletionStatus() ���� ����° ���ڸ� ���� �ش� CompletionKey �� �Ѱܹ��� �� �ִ�.
	// �Ʒ� �ڵ忡���� this �� ���� ClientSession ��ü�� �����͸� �Ѱ��־�
	// �ش� ��Ĺ�� I/O �۾��� �Ͼ ���,
	// GetQueuedCompletionStatus() �� ���� �ش� Ŭ���̾�Ʈ ��ü�� �����͸� �Ѱܹ��� �� �ִ�.
	HANDLE hCP = CreateIoCompletionPort((HANDLE)mSocket, GIocpManager->GetCPHandle(), (ULONG_PTR)this, 0);

	// except error for cp handle
	if (hCP != GIocpManager->GetCPHandle())
	{
		cout << "CreateIoCompletionPort error: " << GetLastError() << endl;
		return false;
	}

	// memcpy to member value(mClientAddr)
	memcpy(&mClientAddr, addr, sizeof(SOCKADDR_IN));

	// is connected
	mIsConnected = true;

	cout << " Client Connected: IP = " << inet_ntoa(mClientAddr.sin_addr) << " PORT = " << ntohs(mClientAddr.sin_port) << endl;

	GSessionManager->IncreaseClientCount();
}

/*
�Լ��� : IsConnected()
��� : Ŭ���̾�Ʈ�� ������� Ȯ��.
*/
bool ClientSession::IsConnected() const
{
	return mIsConnected;
}

/*
�Լ��� : Recv()
��� : Ŭ���̾�Ʈ�� ������ ������ ���ſ�û
*/
bool ClientSession::Recv()
{
	// except error
	if (!IsConnected())
		return false;

	DWORD flags = 0;
	DWORD recvBytes = 0;
	mRecvOverlapped.mWSABuf.buf = mRecvOverlapped.mBuffer;
	mRecvOverlapped.mWSABuf.len = MAX_BUFSIZE;
	mRecvOverlapped.mIOType = IOTYPE::IO_RECV;

	// WSARecv : WSA_IO_PENDING - success message
	if (SOCKET_ERROR == WSARecv(mSocket, &mRecvOverlapped.mWSABuf, 1, &recvBytes, &flags, (LPWSAOVERLAPPED)&mRecvOverlapped, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "recv error.. : " << WSAGetLastError() << endl;
			return false;
		}
	}

	// cout << "Recv Message : " << recvOV->mWSABuf.buf << endl;
	// cout << "Recv Bytes : " << recvBytes << endl;
	
	return true;
}

/*
�Լ��� : Send()
��� : ���ڷ� ���޹��� ���۸� Ŭ���̾�Ʈ�� ����
*/
bool ClientSession::Send()
{
	// except error
	if (!IsConnected())
		return false;
	
	DWORD flags = 0;
	DWORD sendBytes = 0;
	mSendOverlapped.mIOType = IOTYPE::IO_SEND;

	// WSASend : WSA_IO_PENDING - success message
	if (WSASend(mSocket, &mSendOverlapped.mWSABuf, 1, &sendBytes, flags, (LPOVERLAPPED)&mSendOverlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "send error..." << endl;
			return false;
		}
	}
	//cout << "send massage : " << endl;

	return true;
}

/*
�Լ��� : DisConnect()
��� : ���� ����� SOCKET ����
*/
bool ClientSession::DisConnect()
{
	if (!IsConnected())
		return false;

	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

	/// no TCP TIME_WAIT
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)& lingerOption, sizeof(LINGER)))
	{
		cout << "linger option error..." << endl;
	}

	cout << " Client Disconnected: IP = " << inet_ntoa(mClientAddr.sin_addr) << " PORT = " << ntohs(mClientAddr.sin_port) << endl;

	GSessionManager->DecreaseClientCount();

	closesocket(mSocket);

	mIsConnected = false;

	return true;
}

bool ClientSession::SetSendOverlapped(char* buffer)
{
	if (buffer == nullptr)
	{
		return false;
	}

	mSendOverlapped.mWSABuf.len = MAX_BUFSIZE;
	mSendOverlapped.mWSABuf.buf = mSendOverlapped.mBuffer;

	return true;
}