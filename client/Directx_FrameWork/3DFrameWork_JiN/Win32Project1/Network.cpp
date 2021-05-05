#include "Network.h"
#include "PacketManager.h"

SOCKET Network::m_ClientSocket = NULL;

Network* g_pNetwork = nullptr;

Network::Network() : m_PlayerIndex(-1)
{
}

Network::~Network()
{
	Clean();
}

void Network::Init()
{
	WSAStartup(MAKEWORD(2, 2), &m_WsaData);//윈속 초기화
	Set_TCPSocket();
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
	//sChat.mHead.m_Cmd = (UCHAR)PROTOCOL::TEST_CHAT;
	//sChat.mHead.mDataSize = sizeof(SCHAT);

	//char msg[MAX_MSG_LEN] = "";
	//while (true)
	//{
	//	//gets_s(sChat.m_Buffer, MAX_MSG_LEN);
	//	gets_s(sChat.m_Buffer, MAX_MSG_LEN);

	//	// str copy
	//	//memcpy_s(overlapped->mBuffer, MAX_BUFSIZE, (char*)&sChat, sizeof(sChat));
	//	memcpy_s(overlapped->mBuffer, MAX_BUFSIZE, (void*)&sChat, sizeof(SCHAT));

	//	cout << overlapped->mBuffer << endl;

	//	overlapped->mWSABuf.m_Buffer = overlapped->mBuffer;
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

void Network::Connect(short portNum)
{
	cout << "connect()" << endl;

	SOCKADDR_IN servAddr = { 0 };

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servAddr.sin_port = htons(portNum);

	if (connect(m_ClientSocket, (struct sockaddr*) & servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		perror("connect() error");
}

void Network::Run()
{
	Init();
	Connect(PORT_NUM);

	StartRecvThread();
	StartPacketProcessThread();
}

bool Network::StartRecvThread()
{
	// create thread
	DWORD dwThreadId;
	// begin thread
	HANDLE hRecvThread = (HANDLE)_beginthreadex(NULL, 0, recvThread, NULL, 0, (unsigned int*)&dwThreadId);

	// except error - for create thread
	if (hRecvThread == INVALID_HANDLE_VALUE)
	{
		cout << "Create Thread Fail... " << endl;
		return false;
	}
	cout << "Created Recv Thread Thread" << endl;

	CloseHandle(hRecvThread);

	return true;
}

bool Network::StartPacketProcessThread()
{
	// create thread
	DWORD dwThreadId;
	// begin thread
	HANDLE hPacketThread = (HANDLE)_beginthreadex(NULL, 0, packetProcessThread, NULL, 0, (unsigned int*)&dwThreadId);

	// except error - for create thread
	if (hPacketThread == INVALID_HANDLE_VALUE)
	{
		cout << "Create Thread Fail... " << endl;
		return false;
	}
	cout << "Created Packet Thread Thread" << endl;

	CloseHandle(hPacketThread);

	return true;
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

bool Network::SendPacket(PROTOCOL protocol, char* data, DWORD dataSize, bool inGame)
{
	// [헤드] 구조체 생성
	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	head.m_Cmd = (unsigned char)protocol;
	head.m_PacketSize = (DWORD)sizeof(SHEAD) + dataSize;
	head.m_IsTransferToInGame = inGame;

	// [헤드] + [데이터] 조립
	char buffer[MAX_BUFSIZE];
	int bufferSize = head.m_PacketSize;
	memset(buffer, 0, MAX_BUFSIZE);

	memcpy(buffer, (char*)&head, sizeof(SHEAD));

	if (data != NULL)
		memcpy(buffer + sizeof(SHEAD), data, dataSize);

	// 완성된 패킷 전송
	if (SOCKET_ERROR == send(m_ClientSocket, buffer, bufferSize, 0))
	{
		cout << "send packet error.." << endl;
		return false;
	}

	return true;
}

unsigned int WINAPI Network::recvThread(LPVOID lpParam)
{
	while (true)
	{
		char* recvBuffer = new char[MAX_BUFSIZE];
		memset(recvBuffer, 0, MAX_BUFSIZE);
		if (SOCKET_ERROR == recv(m_ClientSocket, recvBuffer, MAX_BUFSIZE, 0))
		{
			cout << "recv packet error.." << endl;
			return false;
		}

		if (g_pPacketManager != nullptr)
		{
			g_pPacketManager->Enqueue(recvBuffer);
		}		
	}

	return 0;
}

unsigned int WINAPI Network::packetProcessThread(LPVOID lpParam)
{
	if (g_pPacketManager != nullptr)
	{
		g_pPacketManager->ProcessAllQueue();
	}

	return 0;
}