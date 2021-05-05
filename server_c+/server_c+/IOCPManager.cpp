#include "IOCPManager.h"

IOCPManager* g_pIocpManager = nullptr;

IOCPManager::IOCPManager() : m_CP(NULL), m_ThreadCount(2), m_ListenSocket(NULL)
{
}

IOCPManager::~IOCPManager()
{
}

bool IOCPManager::InitIOCPServer()
{
	// read system infomation - for count of cpu core(processors)
	SYSTEM_INFO sysinfo;
	SecureZeroMemory(&sysinfo, sizeof(sysinfo));
	GetSystemInfo(&sysinfo);

	// count of thread = core * 2
	m_ThreadCount = sysinfo.dwNumberOfProcessors * 2;
	cout << "Thread count : " << m_ThreadCount << endl;

	// initialize winsock
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// create completion port in first time
	m_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// except for create completion port
	if (m_CP == NULL)
	{
		cout << "Completion Port Create Fail..." << endl;
		return false;
	}

	// create socket for listen
	m_ListenSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	// except for create listening socket
	if (m_ListenSocket == NULL)
	{
		cout << "Socket Create Fail..." << endl;
		return false;
	}

	// server address setting
	SOCKADDR_IN serveraddr;
	SecureZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT_NUM);

	// setting socket options
	int opt = 1;
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)& opt, sizeof(int));

	// bind listening socket
	if (SOCKET_ERROR == ::bind(m_ListenSocket, (SOCKADDR*)& serveraddr, sizeof(serveraddr)))
	{
		cout << "bind() Fail..." << endl;
		return false;
	}

	cout << "IOCP Server Init Succese..." << endl;

	return true;
}

bool IOCPManager::StartIOCPThread()
{
	// create thread
	for (int i = 0; i < m_ThreadCount; ++i)
	{
		DWORD dwThreadId;
		// begin thread
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, workerThread, (LPVOID)i, 0, (unsigned int*)& dwThreadId);

		// except error - for create thread
		if (hThread == INVALID_HANDLE_VALUE)
		{
			cout << "Create Thread Fail... " << endl;
			return false;
		}
		cout << "Created Thread No. " << i + 1 << endl;

		// close handle after create
		CloseHandle(hThread);
	}
	return true;
}

bool IOCPManager::StartPacketProcessThread()
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
	cout << "Created Packet Thread" << endl;

	CloseHandle(hPacketThread);

	return true;
}

bool IOCPManager::StartMatchProcessThread()
{
	// create thread
	DWORD dwThreadId;
	// begin thread
	HANDLE hMatchThread = (HANDLE)_beginthreadex(NULL, 0, matchProcessThread, NULL, 0, (unsigned int*)&dwThreadId);

	// except error - for create thread
	if (hMatchThread == INVALID_HANDLE_VALUE)
	{
		cout << "Create Thread Fail... " << endl;
		return false;
	}
	cout << "Created Match Thread" << endl;

	CloseHandle(hMatchThread);
}

bool IOCPManager::StartInGameProcessThread()
{
	// create thread
	DWORD dwThreadId;
	// begin thread
	HANDLE hInGameThread = (HANDLE)_beginthreadex(NULL, 0, inGameProcessThread, NULL, 0, (unsigned int*)&dwThreadId);

	// except error - for create thread
	if (hInGameThread == INVALID_HANDLE_VALUE)
	{
		cout << "Create Thread Fail... " << endl;
		return false;
	}
	cout << "Created In Game Thread" << endl;

	CloseHandle(hInGameThread);
}

bool IOCPManager::StartSystemUsageThread()
{
	// create thread
	DWORD dwThreadId;
	// begin thread
	HANDLE hSystemThread = (HANDLE)_beginthreadex(NULL, 0, systemUsageThread, NULL, 0, (unsigned int*)&dwThreadId);

	// except error - for create thread
	if (hSystemThread == INVALID_HANDLE_VALUE)
	{
		cout << "Create Thread Fail... " << endl;
		return false;
	}
	cout << "Created System Usage Thread" << endl;

	CloseHandle(hSystemThread);
}

bool IOCPManager::CloseIOCPServer()
{
	// close handle and socket
	CloseHandle(m_CP);
	WSACleanup();
	return true;
}

bool IOCPManager::AcceptLoop()
{
	// listen()
	if (SOCKET_ERROR == listen(m_ListenSocket, SOMAXCONN))
	{
		cout << "listen() Fail..." << endl;
		return false;
	}

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);

	// accept loop
	while (true)
	{
		SOCKET acceptedSock = WSAAccept(m_ListenSocket, (struct sockaddr*)&clientaddr, &addrlen, NULL, NULL);

		if (acceptedSock == INVALID_SOCKET)
		{
			printf_s("accept: invalid socket\n");
			continue;
		}
		
		//getpeername(acceptedSock, (SOCKADDR*)& clientaddr, &addrlen);

		// ���� ���� ����ü �Ҵ�� �ʱ�ȭ
		ClientSession* client = g_pSessionManager->CreateClientSession(acceptedSock);

		// Ŭ���̾�Ʈ ���� ó��
		if (false == client->OnConnect(&clientaddr))
		{
			cout << "[FAIL] : IOCPManager > AcceptLoop() > client->OnConnect() is false" << endl;
			// OnConnect() ���� �ÿ� �ٽ� client ��ü �ʱ�ȭ �� ����
			client->DisConnect();
			g_pSessionManager->DeleteClientSession(client);
		}

		client->Recv();
	}

	return true;
}

unsigned int WINAPI IOCPManager::workerThread(LPVOID lpParam)
{
	// �۷ι� ��ü���� Completion Port Handle ������ -> GetQueuedCompletionStatus() �� Ȱ��
	HANDLE hCP = g_pIocpManager->GetCPHandle();

	while (true)
	{
		DWORD dwBytesTransferred = 0;	// ���۹��� ������
		SOVERLAPPED* overlapped = nullptr;
		ClientSession* client = nullptr;

		int ret = GetQueuedCompletionStatus(hCP, &dwBytesTransferred, (PULONG_PTR)&client, (LPOVERLAPPED*)&overlapped, -1);

		// except error
		// GetQueuedCompletionStatus()
		// ��ȯ : zero or nonzero
		// zero - error / nonzero - success
		if (ret == 0 && GetLastError() == WAIT_TIMEOUT)		// WAIT_TIMEOUT : �ð��ʰ�
		{
			cout << "Thread Time Out" << endl;
			continue;
		}

		if (ret == 0 || dwBytesTransferred == 0)			// dwBytesTransferred == 0 : ���۹��� �����Ͱ� ����
		{
			cout << "Recive Data is zero..." << endl;

			disconnectCompletion(client, overlapped, dwBytesTransferred);

			continue;
		}

		if (overlapped == nullptr)		// overlapped == nullptr : ���۹��� �����Ͱ� ����
		{
			cout << "Recive Data is zero...Overlapped error" << endl;
			client->DisConnect();
			g_pSessionManager->DeleteClientSession(client);
		}

		// GetQueuedCompletionStatus() ������
		// dwBytesTransferred, overlapped �� ���� �޾ƿ� �ش� ������ ���� ó�� ����
		bool completionOK = true;

		switch (overlapped->m_IOType)
		{
		case IOTYPE::IO_SEND:
			// �Ѱܹ��� overlapped ����ü�� I/O type �� SEND �� ��� ������ �Լ�
			completionOK = sendCompletion(client, overlapped, dwBytesTransferred);
			break;

		case IOTYPE::IO_RECV:
			// �Ѱܹ��� overlapped ����ü�� I/O type �� RECV �� ��� ������ �Լ�
			completionOK = receiveCompletion(client, overlapped, dwBytesTransferred);
			break;

		case IOTYPE::IO_DISCONNECT:
			completionOK = disconnectCompletion(client, overlapped, dwBytesTransferred);
			break;

		default:
			break;
		}

		//if (!completionOK)
		//{
		//	cout << "Completion Error..." << endl;
		//	client->DisConnect();
		//	g_pSessionManager->DeleteClientSession(client);
		//}
	}
	
	return 0;
}

unsigned int WINAPI IOCPManager::packetProcessThread(LPVOID lpParam)
{
	g_pPacketManager->ProcessAllQueue();

	return 0;
}

unsigned int WINAPI IOCPManager::matchProcessThread(LPVOID lpParam)
{
	MatchManager::GetInstance()->ProcessMatchList();
	
	return 0;
}

unsigned int WINAPI IOCPManager::inGameProcessThread(LPVOID lpParam)
{
	InGameManager::GetInstance()->ProcessAllQueue();

	return 0;
}

unsigned int WINAPI IOCPManager::systemUsageThread(LPVOID lpParam)
{
	while (true)
	{
		char log[128];
		sprintf(log, "CPU : %.2f %% / Memory : %.2f MB", g_pSystemUsage->GetCpuUsage(), g_pSystemUsage->GetMemoryUsage());

		cout << "[INFO] Usage > " << log << endl;

		g_pUsageLogger->FileWrite(LOGGER_LEVEL::info, log);

		Sleep(1500);
	}

	return 0;
}


bool IOCPManager::receiveCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred)
{
	if (client == nullptr)
	{
		cout << "Recive Client is nullptr" << endl;
		return false;
	}

	// 1. ���� ���� �����ŭ ������ ũ�� ����
	client->CompleteRecv(dwBytesTransferred);

	// 2. ������ ��Ŷ���� Ȯ��
	// 2-1. ������ ��Ŷ�̸�, ������ ó�� ����
	// 2-2. ������ ��Ŷ�� �ƴϸ�, ������ ó�� �����ϰ� �ٽ� Recv ���·� �Ѿ
	ClientPacket pack;

	pack.m_Session = client;
	bool popResult = client->PopBuffer(pack.m_Buffer);	

	// �Ϸ�� pop data �� ���� ó�� �κ� ////////////////////
	if (popResult)
		g_pPacketManager->Enqueue(pack);
	/////////////////////////////////////////////////////////

	return client->Recv();
}

bool IOCPManager::sendCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred)
{
	if (client == nullptr)
	{
		printf_s("sendCompletion client returned nullptr! \n");
		return false;
	}
	
	// send �ÿ� �߰� ó�� �κ� ///////////////////

	///////////////////////////////////////////////

	// ���� �� �Ǿ����� Ȯ��
	if (overlapped->m_WSABuf.len != dwBytesTransferred)
	{
		cout << "send() isn't complete.." << endl;
		return false;
	}

	return true;
}

bool IOCPManager::disconnectCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred)
{
	if (client == nullptr)
	{
		printf_s("disconnectCompletion client returned nullptr! \n");
		return false;
	}

	// ���� ���� ���� ó���ؾ��� ��
	// if in game : �ΰ��� �濡�� ������
	// �ΰ����� ����Ǿ����� �˷��ִ� ��Ŷ ������뿡�� ����
	if (client->GetRoomNum() >= 0)
	{
		InGameManager::GetInstance()->GameEnd(client->GetRoomNum(), GAMEEND_STATE::DISCONNECTED);
		InGameManager::GetInstance()->OutGame(client->GetRoomNum());
	}	

	if (client->GetIsLogin())
	{
		if (MatchManager::GetInstance()->CheckExistClient(client))
		{
			MatchManager::GetInstance()->DeleteClient(client);
		}
		g_pGameDBManager->Logout(client);
	}

	// disconnet
	client->DisConnect();
	g_pSessionManager->DeleteClientSession(client);

	return true;
}