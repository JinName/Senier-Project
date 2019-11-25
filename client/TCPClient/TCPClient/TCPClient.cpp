#include "TCPClient.h"

void TCPClient::Init()
{
	WSAStartup(MAKEWORD(2, 2), &m_WsaData);//윈속 초기화
	Set_TCPSocket();
	Connect(PORT_NUM);
}

void TCPClient::Update()
{
	// overlapped
	SOVERLAPPED* overlapped = new SOVERLAPPED();

	memset(overlapped, 0, sizeof(SOVERLAPPED));

	DWORD flags = 0;
	DWORD sendBytes = 0;
	overlapped->mIOType = IOTYPE::IO_RECV;

	SCHAT sChat;
	memset(&sChat, 0, sizeof(SCHAT));
	sChat.mHead.mCmd = (UCHAR)PROTOCOL::TEST_CHAT;
	sChat.mHead.mDataSize = sizeof(SCHAT);

	char msg[MAX_MSG_LEN] = "";
	while (true)
	{
		//gets_s(sChat.buf, MAX_MSG_LEN);
		gets_s(sChat.buf, MAX_MSG_LEN);

		// str copy
		//memcpy_s(overlapped->mBuffer, MAX_BUFSIZE, (char*)&sChat, sizeof(sChat));
		memcpy_s(overlapped->mBuffer, MAX_BUFSIZE, (void*)&sChat, sizeof(SCHAT));

		cout << overlapped->mBuffer << endl;

		overlapped->mWSABuf.buf = overlapped->mBuffer;
		overlapped->mWSABuf.len = sizeof(overlapped->mBuffer);

		if (SOCKET_ERROR == WSASend(m_ClientSocket, &overlapped->mWSABuf, 1, &sendBytes, flags, (LPOVERLAPPED)overlapped, NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				cout << "send error..." << WSAGetLastError() << endl;
				break;
			}
		}

		//send(m_ClientSocket, msg, sizeof(msg), 0);//송신
		if (strcmp(msg, "exit") == 0)
		{
			break;
		}
	}
}

void TCPClient::Clean()
{
	closesocket(m_ClientSocket);//소켓 닫기    

	WSACleanup();//윈속 해제화
}

void TCPClient::Set_TCPSocket()
{
	cout << "set socket()" << endl;

	// m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_ClientSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_ClientSocket == INVALID_SOCKET)
		perror("socket() error");
}

void TCPClient::Connect(short _portNum)
{
	cout << "connect()" << endl;

	SOCKADDR_IN servAddr = { 0 };

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servAddr.sin_port = htons(_portNum);

	if (connect(m_ClientSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		perror("connect() error");
	
	Threading();
}

void TCPClient::Threading()
{
	cout << "begin thread()" << endl;

	_beginthread(RecvThreadPoint, 0, (void*)m_ClientSocket);
}

void RecvThreadPoint(void* param)
{
	cout << "thread function()" << endl;

	SOCKET sock = (SOCKET)param;
	char msg[MAX_MSG_LEN];

	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);

	while (recv(sock, msg, MAX_MSG_LEN, 0)>0)
	{
		printf("%s\n", msg);
	}
	closesocket(sock);
}

// TCPClient.cpp

//#include "stdafx.h"
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//
//int main(int argc, char* argv[])
//{
//	WSADATA wsaData;
//	SOCKET hSocket;
//	SOCKADDR_IN servAddr;
//
//	int port = 5001;
//	char message[30];
//	int strLen;
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		ErrorHandling("WSAStartup() error!");
//
//	hSocket = socket(PF_INET, SOCK_STREAM, 0);
//
//	if (hSocket == INVALID_SOCKET)
//		ErrorHandling("hSocket() error!");
//
//	memset(&servAddr, 0, sizeof(servAddr));
//
//	servAddr.sin_family = AF_INET;
//	servAddr.sin_port = htons(port);
//	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);
//
//	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
//		ErrorHandling("connect() error!");
//
//	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
//
//	if (strLen == -1)
//		ErrorHandling("read() error!");
//
//	printf("message from server: %s \n", message);
//
//	closesocket(hSocket);
//	WSACleanup();
//
//	return 0;
//}