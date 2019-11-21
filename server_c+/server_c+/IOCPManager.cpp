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

		// ���� ���� ����ü �Ҵ�� �ʱ�ȭ
		ClientSession* client = GSessionManager->CreateClientSession(acceptedSock);

		// Ŭ���̾�Ʈ ���� ó��
		if (false == client->OnConnect(&clientaddr))
		{
			// OnConnect() ���� �ÿ� �ٽ� client ��ü �ʱ�ȭ �� ����
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
	// �۷ι� ��ü���� Completion Port Handle ������ -> GetQueuedCompletionStatus() �� Ȱ��
	HANDLE hCP = GIocpManager->GetCPHandle();

	cout << "start thread CP " << hCP << endl;

	while (true)
	{
		DWORD dwBytesTransferred;	// ���۹��� ������
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
			client->DisConnect();
			GSessionManager->DeleteClientSession(client);
			continue;
		}

		if (overlapped == nullptr)		// overlapped == nullptr : ���۹��� �����Ͱ� ����
		{
			cout << "Recive Data is zero...Overlapped error" << endl;
			client->DisConnect();
			GSessionManager->DeleteClientSession(client);
		}

		// GetQueuedCompletionStatus() ������
		// dwBytesTransferred, overlapped �� ���� �޾ƿ� �ش� ������ ���� ó�� ����
		bool completionOK = true;

		switch (overlapped->mIOType)
		{
		case IOTYPE::IO_SEND:
			// �Ѱܹ��� overlapped ����ü�� I/O type �� SEND �� ��� ������ �Լ�
			//completionOK = SendCompletion(client, overlapped, dwBytesTransferred);
			break;

		case IOTYPE::IO_RECV:
			// �Ѱܹ��� overlapped ����ü�� I/O type �� RECV �� ��� ������ �Լ�
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

	// echo back ���� overlapped ����ü�� ������� �ʰ�,
	// recv �Լ����� WSARecv() �� ���� ���ο� ����ü ������ ������
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

	/// ���� �� �Ǿ����� Ȯ���ϴ� �� ó��..
	if (overlapped->mWSABuf.len != dwBytesTransferred)
	{
		delete overlapped;
		return false;
	}

	delete overlapped;

	return true;
}