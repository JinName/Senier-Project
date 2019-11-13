#include "ClientSession.h"
#include "IOCPManager.h"
#include "SessionManager.h"

ClientSession::ClientSession(SOCKET sock) : mIsConnected(false), mSocket(sock)
{
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));	// use memset to initialize sockaddr_in value
}

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

	// GSessionManager->IncreaseClientCount();
}

bool ClientSession::IsConnected()
{
	return mIsConnected;
}

bool ClientSession::Recv()
{
	// except error
	if (!IsConnected())
		return false;

	// create overlapped struct for recv
	stOverlapped* recvOV = new stOverlapped(this, IO_RECV);

	// setting for recv
	DWORD flags = 0;
	DWORD recvBytes = 0;
	recvOV->mWSABuf.buf = recvOV->mBuffer;
	recvOV->mWSABuf.len = MAX_BUFSIZE;

	// WSARecv : WSA_IO_PENDING - success message
	if (SOCKET_ERROR == WSARecv(mSocket, &recvOV->mWSABuf, 1, &recvBytes, &flags, recvOV, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "recv error.." << endl;
			return false;
		}
	}

	cout << "Recv Message : " << endl;

	return true;
}

bool ClientSession::Send(const char* buf, int len)
{
	// except error
	if (!IsConnected())
		return false;

	// create overlapped struct for send
	stOverlapped* sendOV = new stOverlapped(this, IO_SEND);
	
	// setting for send
	memcpy_s(sendOV->mBuffer, MAX_BUFSIZE, buf, len);
	
	DWORD flags = 0;
	DWORD sendBytes = 0;
	sendOV->mWSABuf.buf = sendOV->mBuffer;
	sendOV->mWSABuf.len = len;

	// WSASend : WSA_IO_PENDING - success message
	if (WSASend(mSocket, &sendOV->mWSABuf, 1, &sendBytes, flags, sendOV, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "send error..." << endl;
			return false;
		}
	}
	cout << "send massage : " << endl;

	return true;
}

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

	//GSessionManager->DecreaseClientCount();

	closesocket(mSocket);

	mIsConnected = false;

	return true;
}