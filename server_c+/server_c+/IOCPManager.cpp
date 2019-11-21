#include "IOCPManager.h"

IOCPManager* GIocpManager = nullptr;

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
	if (SOCKET_ERROR == bind(mListenSocket, (SOCKADDR*)& serveraddr, sizeof(serveraddr)))
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
		ClientSession* client = GSessionManager->CreateClientSession(acceptedSock);

		// 클라이언트 접속 처리
		if (false == client->OnConnect(&clientaddr))
		{
			// OnConnect() 실패 시에 다시 client 객체 초기화 후 삭제
			client->DisConnect();
			GSessionManager->DeleteClientSession(client);
		}

		//SOVERLAPPED* recvOV = new SOVERLAPPED(IO_RECV);

		client->Recv(NULL);
	}

	return true;
}

unsigned int WINAPI IOCPManager::WorkerThread(LPVOID lpParam)
{
	// 글로벌 객체에서 Completion Port Handle 가져옴 -> GetQueuedCompletionStatus() 에 활용
	HANDLE hCP = GIocpManager->GetCPHandle();

	cout << "start thread CP " << hCP << endl;

	while (true)
	{
		DWORD dwBytesTransferred;	// 전송받을 데이터
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
			client->DisConnect();
			GSessionManager->DeleteClientSession(client);
			continue;
		}

		if (overlapped == nullptr)		// overlapped == nullptr : 전송받은 데이터가 없음
		{
			cout << "Recive Data is zero...Overlapped error" << endl;
			client->DisConnect();
			GSessionManager->DeleteClientSession(client);
		}

		// GetQueuedCompletionStatus() 성공시
		// dwBytesTransferred, overlapped 의 값을 받아와 해당 정보에 대한 처리 가능
		bool completionOK = true;

		switch (overlapped->mIOType)
		{
		case IOTYPE::IO_SEND:
			// 넘겨받은 overlapped 구조체의 I/O type 이 SEND 일 경우 수행할 함수
			//completionOK = SendCompletion(client, overlapped, dwBytesTransferred);
			break;

		case IOTYPE::IO_RECV:
			// 넘겨받은 overlapped 구조체의 I/O type 이 RECV 일 경우 수행할 함수
			completionOK = ReceiveCompletion(client, overlapped, dwBytesTransferred);
			break;

		default:
			break;
		}

		if (!completionOK)
		{
			cout << "Completion Error..." << endl;
			client->DisConnect();
			GSessionManager->DeleteClientSession(client);
		}
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

	// echo back
	// bool sendRe = client->Send(overlapped->mBuffer, dwBytesTransferred);

	// echo back 이후 overlapped 구조체는 사용하지 않고,
	// recv 함수에서 WSARecv() 를 통해 새로운 구조체 정보를 가져옴
	// delete overlapped;

	//if (!sendRe)
	//{
	//	cout << "send fail..." << endl;
	//}

	return client->Recv(overlapped);
}

bool IOCPManager::SendCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred)
{
	if (client == nullptr)
	{
		printf_s("SendCompletion client returned nullptr! \n");
		return false;
	}

	/// 전송 다 되었는지 확인하는 것 처리..
	if (overlapped->mWSABuf.len != dwBytesTransferred)
	{
		delete overlapped;
		return false;
	}

	delete overlapped;

	return true;
}