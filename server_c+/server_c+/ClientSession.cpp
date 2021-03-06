#include "ClientSession.h"
#include "IOCPManager.h"
#include "SessionManager.h"

ClientSession::ClientSession(SOCKET sock) : mIsConnected(false), mSocket(sock), mRoomNum(-1), mIsLogin(false)
{
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));	// use memset to initialize sockaddr_in value
	memset(&mRecvOverlapped, 0, sizeof(SOVERLAPPED));
	memset(&mSendOverlapped, 0, sizeof(SOVERLAPPED));

	memset(mID, 0, MAX_ID_LEN);
}

/*
함수명 : OnConnect()

인자값 :
1) SOCKADDR_IN* addr : 클라이언트 주소

기능 : 전달받은 클라이언트 주소를 저장,
	   전달받은 주소에 대한 클라이언트가 존재하는지 확인.
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
	// 세번째 인자인 CompletionKey 를 통해 원하는 변수를 넘길경우,
	// GetQueuedCompletionStatus() 에서 세번째 인자를 통해 해당 CompletionKey 를 넘겨받을 수 있다.
	// 아래 코드에서는 this 를 통해 ClientSession 객체의 포인터를 넘겨주어
	// 해당 소캣에 I/O 작업이 일어날 경우,
	// GetQueuedCompletionStatus() 을 통해 해당 클라이언트 객체의 포인터를 넘겨받을 수 있다.
	HANDLE hCP = CreateIoCompletionPort((HANDLE)mSocket, g_pIocpManager->GetCPHandle(), (ULONG_PTR)this, 0);

	// except error for cp handle
	if (hCP != g_pIocpManager->GetCPHandle())
	{
		cout << "CreateIoCompletionPort error: " << GetLastError() << endl;
		return false;
	}

	// memcpy to member value(mClientAddr)
	memcpy(&mClientAddr, addr, sizeof(SOCKADDR_IN));

	// is connected
	mIsConnected = true;

	cout << " Client Connected: IP = " << inet_ntoa(mClientAddr.sin_addr) << " PORT = " << ntohs(mClientAddr.sin_port) << endl;

	// write log
	char log[128];
	sprintf(log, "Client Connected IP = %s / PORT = %d", inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port));
	g_pConnLogger->file_write(LOGGER_LEVEL::info, log);

	g_pSessionManager->IncreaseClientCount();

	return true;
}

/*
함수명 : IsConnected()
기능 : 클라이언트의 연결상태 확인.
*/
bool ClientSession::IsConnected() const
{
	return mIsConnected;
}

/*
함수명 : Recv()
기능 : 클라이언트가 전송한 데이터 수신요청
*/
bool ClientSession::Recv()
{
	// except error
	if (!IsConnected())
		return false;	

	if (mRingBuffer.GetAvailableBufferSize() == 0)
	{
		cout << "[return false] : ClientSession >> Recv() / less buffer size" << endl;
		return false;
	}

	DWORD flags = 0;
	DWORD recvBytes = 0;
	//mRecvOverlapped.mWSABuf.buf = mRecvOverlapped.mBuffer;
	//mRecvOverlapped.mWSABuf.len = MAX_BUFSIZE;
	mRecvOverlapped.mWSABuf.buf = mRingBuffer.GetWritablePointer();
	mRecvOverlapped.mWSABuf.len = mRingBuffer.GetAvailableBufferSize();
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

	return true;
}

/*
함수명 : Send()
기능 : 인자로 전달받은 버퍼를 클라이언트에 전송
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

	return true;
}

/*
함수명 : DisConnect()
기능 : 접속 종료된 SOCKET 삭제
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

	// write log
	char log[128];
	sprintf(log, "Client Disconnected IP = %s / PORT = %d", inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port));
	g_pConnLogger->file_write(LOGGER_LEVEL::info, log);

	g_pSessionManager->DecreaseClientCount();

	closesocket(mSocket);

	mIsConnected = false;

	return true;
}

bool ClientSession::SetSendOverlapped()
{
	mSendOverlapped.mWSABuf.len = MAX_BUFSIZE;
	mSendOverlapped.mWSABuf.buf = mSendOverlapped.mBuffer;

	return true;
}

bool ClientSession::SetSendOverlapped(char* buffer, int bufferSize)
{
	if (buffer == nullptr)
	{
		return false;
	}

	memcpy(mSendOverlapped.mBuffer, buffer, bufferSize);
	mSendOverlapped.mWSABuf.len = MAX_BUFSIZE;
	mSendOverlapped.mWSABuf.buf = mSendOverlapped.mBuffer;

	return true;
}

void ClientSession::CompleteRecv(DWORD _dataSize)
{
	mRingBuffer.CommitDataSize(_dataSize);
}

bool ClientSession::PopBuffer(char* _outBuffer)
{
	// head data peek 을 위한 임시 배열
	char tempHead[sizeof(SHEAD)];
	memset(tempHead, 0, sizeof(SHEAD));

	// peek : Head 길이 만큼 Peek 해서 Pop 해야하는 데이터 길이 확인
	bool peekResult = mRingBuffer.Peek(tempHead, sizeof(SHEAD));

	// exception
	// 링버퍼에 데이터가 Head 길이만큼 들어와있지 않으면
	// return false;
	if (peekResult == false)
	{
		cout << "[return false] ClientSession >> PopBuffer() : buffer len < request peek len" << endl;
		return false;
	}

	// packSize : Pop() 해야하는 길이
	DWORD packSize = PacketManager::GetInstance()->GetTotalPacketSize(tempHead, sizeof(SHEAD));

	// pop : 실질적인 데이터를 Pop 함
	bool popResult = mRingBuffer.Pop(_outBuffer, packSize);
	
	// exception
	// Pop() 해야하는 데이터 길이만큼 링버퍼에 존재하지 않을 경우
	// return false;
	if (popResult == false)
	{
		cout << "[return false] ClientSession >> PopBuffer() : buffer len < request pop len" << endl;
		return false;
	}

	return true;
}