#include "IOCPManager.h"

IOCPManager* GIocpManager = nullptr;

IOCPManager::IOCPManager() : mThreadCount(2)
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
	mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

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

unsigned int WINAPI IOCPManager::WorkerThread(LPVOID lpParam)
{
	return 0;
}