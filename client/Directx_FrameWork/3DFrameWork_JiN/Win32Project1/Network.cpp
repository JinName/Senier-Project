#include "Network.h"

void Network::Init()
{
	WSAStartup(MAKEWORD(2, 2), &m_WsaData);//윈속 초기화
	Set_TCPSocket();
	Connect(PORT_NUM);
}

void Network::Update()
{
	//// overlapped
	//SOVERLAPPED* overlapped = new SOVERLAPPED();

	//memset(overlapped, 0, sizeof(SOVERLAPPED));

	//DWORD flags = 0;
	//DWORD sendBytes = 0;
	//overlapped->mIOType = IOTYPE::IO_RECV;

	//SCHAT sChat;
	//memset(&sChat, 0, sizeof(SCHAT));
	//sChat.mHead.mCmd = (UCHAR)PROTOCOL::TEST_CHAT;
	//sChat.mHead.mDataSize = sizeof(SCHAT);

	//char msg[MAX_MSG_LEN] = "";
	//while (true)
	//{
	//	//gets_s(sChat.buf, MAX_MSG_LEN);
	//	gets_s(sChat.buf, MAX_MSG_LEN);

	//	// str copy
	//	//memcpy_s(overlapped->mBuffer, MAX_BUFSIZE, (char*)&sChat, sizeof(sChat));
	//	memcpy_s(overlapped->mBuffer, MAX_BUFSIZE, (void*)&sChat, sizeof(SCHAT));

	//	cout << overlapped->mBuffer << endl;

	//	overlapped->mWSABuf.buf = overlapped->mBuffer;
	//	overlapped->mWSABuf.len = sizeof(overlapped->mBuffer);

	//	if (SOCKET_ERROR == WSASend(m_ClientSocket, &overlapped->mWSABuf, 1, &sendBytes, flags, (LPOVERLAPPED)overlapped, NULL))
	//	{
	//		if (WSAGetLastError() != WSA_IO_PENDING)
	//		{
	//			cout << "send error..." << WSAGetLastError() << endl;
	//			break;
	//		}
	//	}

	//	//send(m_ClientSocket, msg, sizeof(msg), 0);//송신
	//	if (strcmp(msg, "exit") == 0)
	//	{
	//		break;
	//	}
	//}
}

void Network::Clean()
{
	closesocket(m_ClientSocket);//소켓 닫기    

	WSACleanup();//윈속 해제화
}

void Network::Set_TCPSocket()
{
	cout << "set socket()" << endl;

	// m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_ClientSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_ClientSocket == INVALID_SOCKET)
		perror("socket() error");
}

void Network::Connect(short _portNum)
{
	cout << "connect()" << endl;

	SOCKADDR_IN servAddr = { 0 };

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servAddr.sin_port = htons(_portNum);

	if (connect(m_ClientSocket, (struct sockaddr*) & servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		perror("connect() error");

	//Threading();
}

void Network::Run()
{
	Init();
	Set_TCPSocket();
	Connect(PORT_NUM);
}

//void Network::Threading()
//{
//	cout << "begin thread()" << endl;
//
//	_beginthread(RecvThreadPoint, 0, (void*)m_ClientSocket);
//}
//
//void RecvThreadPoint(void* param)
//{
//	cout << "thread function()" << endl;
//
//	SOCKET sock = (SOCKET)param;
//	char msg[MAX_MSG_LEN];
//
//	SOCKADDR_IN cliaddr = { 0 };
//	int len = sizeof(cliaddr);
//
//	while (recv(sock, msg, MAX_MSG_LEN, 0) > 0)
//	{
//		printf("%s\n", msg);
//	}
//	closesocket(sock);
//}

bool Network::SendPacket(char* _buffer, DWORD _bufferSize)
{
	if (SOCKET_ERROR == send(m_ClientSocket, _buffer, _bufferSize, 0))
	{
		cout << "send packet error.." << endl;
		return false;
	}

	return true;
}