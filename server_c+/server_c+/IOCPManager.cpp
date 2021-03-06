#include "IOCPManager.h"

IOCPManager* g_pIocpManager = nullptr;

IOCPManager::IOCPManager() : mCP(NULL), mThreadCount(2), mListenSocket(NULL)
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
	mThreadCount = sysinfo.dwNumberOfProcessors * 2;
	cout << "Thread count : " << mThreadCount << endl;

	// initialize winsock
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// create completion port in first time
	mCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// except for create completion port
	if (mCP == NULL)
	{
		cout << "Completion Port Create Fail..." << endl;
		return false;
	}

	// create socket for listen
	mListenSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	// except for create listening socket
	if (mListenSocket == NULL)
	{
		cout << "Socket Create Fail..." << endl;
		return false;
	}

	// server addr setting
	SOCKADDR_IN serveraddr;
	SecureZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT_NUM);

	// setting socket options
	int opt = 1;
	setsockopt(mListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)& opt, sizeof(int));

	// bind listening socket
	if (SOCKET_ERROR == ::bind(mListenSocket, (SOCKADDR*)& serveraddr, sizeof(serveraddr)))
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
	for (int i = 0; i < mThreadCount; ++i)
	{
		DWORD dwThreadId;
		// begin thread
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, (LPVOID)i, 0, (unsigned int*)& dwThreadId);

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
	HANDLE hPacketThread = (HANDLE)_beginthreadex(NULL, 0, PacketProcessThread, NULL, 0, (unsigned int*)&dwThreadId);

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
	HANDLE hMatchThread = (HANDLE)_beginthreadex(NULL, 0, MatchProcessThread, NULL, 0, (unsigned int*)&dwThreadId);

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
	HANDLE hSystemThread = (HANDLE)_beginthreadex(NULL, 0, SystemUsageThread, NULL, 0, (unsigned int*)&dwThreadId);

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
	CloseHandle(mCP);
	WSACleanup();
	return true;
}

bool IOCPManager::AcceptLoop()
{
	// listen()
	if (SOCKET_ERROR == listen(mListenSocket, SOMAXCONN))
	{
		cout << "listen() Fail..." << endl;
		return false;
	}

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);

	// accept loop
	while (true)
	{
		SOCKET acceptedSock = WSAAccept(mListenSocket, (struct sockaddr*)&clientaddr, &addrlen, NULL, NULL);

		if (acceptedSock == INVALID_SOCKET)
		{
			printf_s("accept: invalid socket\n");
			continue;
		}
		
		//getpeername(acceptedSock, (SOCKADDR*)& clientaddr, &addrlen);

		// 소켓 정보 구조체 할당과 초기화
		ClientSession* client = g_pSessionManager->CreateClientSession(acceptedSock);

		// 클라이언트 접속 처리
		if (false == client->OnConnect(&clientaddr))
		{
			cout << "[FAIL] : IOCPManager > AcceptLoop() > client->OnConnect() is false" << endl;
			// OnConnect() 실패 시에 다시 client 객체 초기화 후 삭제
			client->DisConnect();
			g_pSessionManager->DeleteClientSession(client);
		}

		client->Recv();
	}

	return true;
}

unsigned int WINAPI IOCPManager::WorkerThread(LPVOID lpParam)
{
	// 글로벌 객체에서 Completion Port Handle 가져옴 -> GetQueuedCompletionStatus() 에 활용
	HANDLE hCP = g_pIocpManager->GetCPHandle();

	while (true)
	{
		DWORD dwBytesTransferred = 0;	// 전송받을 데이터
		SOVERLAPPED* overlapped = nullptr;
		ClientSession* client = nullptr;

		int ret = GetQueuedCompletionStatus(hCP, &dwBytesTransferred, (PULONG_PTR)&client, (LPOVERLAPPED*)&overlapped, -1);

		// except error
		// GetQueuedCompletionStatus()
		// 반환 : zero or nonzero
		// zero - error / nonzero - success
		if (ret == 0 && GetLastError() == WAIT_TIMEOUT)		// WAIT_TIMEOUT : 시간초과
		{
			cout << "Thread Time Out" << endl;
			continue;
		}

		if (ret == 0 || dwBytesTransferred == 0)			// dwBytesTransferred == 0 : 전송받은 데이터가 없음
		{
			cout << "Recive Data is zero..." << endl;

			DisconnectCompletion(client, overlapped, dwBytesTransferred);

			continue;
		}

		if (overlapped == nullptr)		// overlapped == nullptr : 전송받은 데이터가 없음
		{
			cout << "Recive Data is zero...Overlapped error" << endl;
			client->DisConnect();
			g_pSessionManager->DeleteClientSession(client);
		}

		// GetQueuedCompletionStatus() 성공시
		// dwBytesTransferred, overlapped 의 값을 받아와 해당 정보에 대한 처리 가능
		bool completionOK = true;

		switch (overlapped->mIOType)
		{
		case IOTYPE::IO_SEND:
			// 넘겨받은 overlapped 구조체의 I/O type 이 SEND 일 경우 수행할 함수
			completionOK = SendCompletion(client, overlapped, dwBytesTransferred);
			break;

		case IOTYPE::IO_RECV:
			// 넘겨받은 overlapped 구조체의 I/O type 이 RECV 일 경우 수행할 함수
			completionOK = ReceiveCompletion(client, overlapped, dwBytesTransferred);
			break;

		case IOTYPE::IO_DISCONNECT:
			completionOK = DisconnectCompletion(client, overlapped, dwBytesTransferred);
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

unsigned int WINAPI IOCPManager::PacketProcessThread(LPVOID lpParam)
{
	PacketManager::GetInstance()->ProcessAllQueue();

	return 0;
}

unsigned int WINAPI IOCPManager::MatchProcessThread(LPVOID lpParam)
{
	MatchManager::GetInstance()->ProcessMatchList();
	
	return 0;
}

unsigned int WINAPI IOCPManager::inGameProcessThread(LPVOID lpParam)
{
	InGameManager::GetInstance()->ProcessAllQueue();

	return 0;
}

unsigned int WINAPI IOCPManager::SystemUsageThread(LPVOID lpParam)
{
	while (true)
	{
		char log[128];
		sprintf(log, "CPU : %.2f %% / Memory : %.2f MB", g_pSystemUsage->GetCpuUsage(), g_pSystemUsage->GetMemoryUsage());

		cout << "[INFO] Usage > " << log << endl;

		g_pUsageLogger->file_write(LOGGER_LEVEL::info, log);

		Sleep(1500);
	}

	return 0;
}


bool IOCPManager::ReceiveCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred)
{
	if (client == nullptr)
	{
		cout << "Recive Client is nullptr" << endl;
		return false;
	}

	// 1. 들어온 버퍼 사이즈만큼 링버퍼 크기 수정
	client->CompleteRecv(dwBytesTransferred);

	// 2. 완전한 패킷인지 확인
	// 2-1. 완전한 패킷이면, 데이터 처리 진행
	// 2-2. 완전한 패킷이 아니면, 데이터 처리 보류하고 다시 Recv 상태로 넘어감
	ClientPacket pack;

	pack.mSession = client;
	bool popResult = client->PopBuffer(pack.mBuffer);	

	// 완료된 pop data 에 대한 처리 부분 ////////////////////
	if (popResult)
		PacketManager::GetInstance()->Enqueue(pack);
	/////////////////////////////////////////////////////////

	return client->Recv();
}

bool IOCPManager::SendCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred)
{
	if (client == nullptr)
	{
		printf_s("SendCompletion client returned nullptr! \n");
		return false;
	}
	
	// send 시에 추가 처리 부분 ///////////////////

	///////////////////////////////////////////////

	// 전송 다 되었는지 확인
	if (overlapped->mWSABuf.len != dwBytesTransferred)
	{
		cout << "send() isn't complete.." << endl;
		return false;
	}

	return true;
}

bool IOCPManager::DisconnectCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred)
{
	if (client == nullptr)
	{
		printf_s("DisconnectCompletion client returned nullptr! \n");
		return false;
	}

	// 연결 해제 전에 처리해야할 것
	// if in game : 인게임 방에서 나가기
	// 인게임이 종료되었음을 알려주는 패킷 대전상대에게 전송
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