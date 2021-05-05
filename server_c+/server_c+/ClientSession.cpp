#include "ClientSession.h"
#include "IOCPManager.h"
#include "SessionManager.h"

ClientSession::ClientSession(SOCKET socket) : m_IsConnected(false), m_Socket(socket), m_RoomNum(-1), m_IsLogin(false)
{
	memset(&m_ClientAddr, 0, sizeof(SOCKADDR_IN));	// use memset to initialize sockaddr_in value
	memset(&m_RecvOverlapped, 0, sizeof(SOVERLAPPED));
	memset(&m_SendOverlapped, 0, sizeof(SOVERLAPPED));

	memset(m_ID, 0, MAX_ID_LEN);
}

/*
�Լ��� : OnConnect()

���ڰ� :
1) SOCKADDR_IN* address : Ŭ���̾�Ʈ �ּ�

��� : ���޹��� Ŭ���̾�Ʈ �ּҸ� ����,
	   ���޹��� �ּҿ� ���� Ŭ���̾�Ʈ�� �����ϴ��� Ȯ��.
*/
bool ClientSession::OnConnect(SOCKADDR_IN* address)
{
	// setting socket I/O mode - not 0 : nonblocking mode / 0 : blocking mode
	u_long arg = 1;
	ioctlsocket(m_Socket, FIONBIO, &arg);

	// setting socket option
	int opt = 1;
	setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)& opt, sizeof(int));

	// setting recv m_Buffer option - (SOCKET, SQL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(int))
	// SQL_SOCKET option
	// SO_RCVBUF, SO_SNDBUF - setting recv or send bufsize
	opt = 0;
	if (SOCKET_ERROR == setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (const char*)& opt, sizeof(int)))
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
	HANDLE hCP = CreateIoCompletionPort((HANDLE)m_Socket, g_pIocpManager->GetCPHandle(), (ULONG_PTR)this, 0);

	// except error for cp handle
	if (hCP != g_pIocpManager->GetCPHandle())
	{
		cout << "CreateIoCompletionPort error: " << GetLastError() << endl;
		return false;
	}

	// memcpy to member value(m_ClientAddr)
	memcpy(&m_ClientAddr, address, sizeof(SOCKADDR_IN));

	// is connected
	m_IsConnected = true;

	cout << " Client Connected: IP = " << inet_ntoa(m_ClientAddr.sin_addr) << " PORT = " << ntohs(m_ClientAddr.sin_port) << endl;

	// write log
	char log[128];
	sprintf(log, "Client Connected IP = %s / PORT = %d", inet_ntoa(m_ClientAddr.sin_addr), ntohs(m_ClientAddr.sin_port));
	g_pConnLogger->FileWrite(LOGGER_LEVEL::info, log);

	g_pSessionManager->IncreaseClientCount();

	return true;
}

/*
�Լ��� : IsConnected()
��� : Ŭ���̾�Ʈ�� ������� Ȯ��.
*/
bool ClientSession::IsConnected() const
{
	return m_IsConnected;
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

	if (m_RingBuffer.GetAvailableBufferSize() == 0)
	{
		cout << "[return false] : ClientSession >> Recv() / less buffer size" << endl;
		return false;
	}

	DWORD flags = 0;
	DWORD recvBytes = 0;
	m_RecvOverlapped.m_WSABuf.len = m_RingBuffer.GetAvailableBufferSize();
	m_RecvOverlapped.m_WSABuf.buf = m_RingBuffer.GetWritablePointer();	
	m_RecvOverlapped.m_IOType = IOTYPE::IO_RECV;	

	// WSARecv : WSA_IO_PENDING - success message
	if (SOCKET_ERROR == WSARecv(m_Socket, &m_RecvOverlapped.m_WSABuf, 1, &recvBytes, &flags, (LPWSAOVERLAPPED)&m_RecvOverlapped, NULL))
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
	m_SendOverlapped.m_IOType = IOTYPE::IO_SEND;

	// WSASend : WSA_IO_PENDING - success message
	if (WSASend(m_Socket, &m_SendOverlapped.m_WSABuf, 1, &sendBytes, flags, (LPOVERLAPPED)&m_SendOverlapped, NULL) == SOCKET_ERROR)
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
	if (SOCKET_ERROR == setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char*)& lingerOption, sizeof(LINGER)))
	{
		cout << "linger option error..." << endl;
	}

	cout << " Client Disconnected: IP = " << inet_ntoa(m_ClientAddr.sin_addr) << " PORT = " << ntohs(m_ClientAddr.sin_port) << endl;

	// write log
	char log[128];
	sprintf(log, "Client Disconnected IP = %s / PORT = %d", inet_ntoa(m_ClientAddr.sin_addr), ntohs(m_ClientAddr.sin_port));
	g_pConnLogger->FileWrite(LOGGER_LEVEL::info, log);

	g_pSessionManager->DecreaseClientCount();

	closesocket(m_Socket);

	m_IsConnected = false;

	return true;
}

bool ClientSession::SetSendOverlapped()
{
	m_SendOverlapped.m_WSABuf.len = MAX_BUFSIZE;
	m_SendOverlapped.m_WSABuf.buf = m_SendOverlapped.m_Buffer;

	return true;
}

bool ClientSession::SetSendOverlapped(char* buffer, int bufferSize)
{
	if (buffer == nullptr)
	{
		return false;
	}

	memcpy(m_SendOverlapped.m_Buffer, buffer, bufferSize);
	m_SendOverlapped.m_WSABuf.len = MAX_BUFSIZE;
	m_SendOverlapped.m_WSABuf.buf = m_SendOverlapped.m_Buffer;

	return true;
}

void ClientSession::CompleteRecv(DWORD dataSize)
{
	m_RingBuffer.CommitDataSize(dataSize);
}

bool ClientSession::PopBuffer(char* outBuffer)
{
	// head data peek �� ���� �ӽ� �迭
	char tempHead[sizeof(SHEAD)];
	memset(tempHead, 0, sizeof(SHEAD));

	// peek : Head ���� ��ŭ Peek �ؼ� Pop �ؾ��ϴ� ������ ���� Ȯ��
	bool peekResult = m_RingBuffer.Peek(tempHead, sizeof(SHEAD));

	// exception
	// �����ۿ� �����Ͱ� Head ���̸�ŭ �������� ������
	// return false;
	if (peekResult == false)
	{
		cout << "[return false] ClientSession >> PopBuffer() : buffer len < request peek len" << endl;
		return false;
	}

	// packSize : Pop() �ؾ��ϴ� ����
	DWORD packSize = g_pPacketManager->GetTotalPacketSize(tempHead, sizeof(SHEAD));

	// pop : �������� �����͸� Pop ��
	bool popResult = m_RingBuffer.Pop(outBuffer, packSize);
	
	// exception
	// Pop() �ؾ��ϴ� ������ ���̸�ŭ �����ۿ� �������� ���� ���
	// return false;
	if (popResult == false)
	{
		cout << "[return false] ClientSession >> PopBuffer() : buffer len < request pop len" << endl;
		return false;
	}

	return true;
}