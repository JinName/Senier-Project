#pragma once
#include "stdafx.h"
#include "SessionManager.h"
#include "ClientSession.h"
#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"
#include "SystemUsage.h"
#include "GameDBManager.h"

class IOCPManager
{
public:
	IOCPManager();
	~IOCPManager();

	bool		InitIOCPServer();	// initialize
	bool		StartIOCPThread();	// start iocp worker thread
	bool		StartPacketProcessThread();	// start packet process thread
	bool		StartMatchProcessThread();	// start match process thread
	bool		StartInGameProcessThread();	// start in gmae process thread
	bool		StartSystemUsageThread();	// start system usage thread
	bool		CloseIOCPServer();	// close iocp server	

	bool		AcceptLoop();		// accept loop

	// get
	HANDLE		GetCPHandle()		{ return mCP; }
	int			GetThreadCount()	{ return mThreadCount; }

private:
	HANDLE		mCP;				// handle - for completion port
	int			mThreadCount;		// count of WorkerThread = sysinfo.dwNumberOfProcessors * 2

	SOCKET		mListenSocket;		// socket - for listen

	// static function - must private.. not to public
	static unsigned int WINAPI WorkerThread(LPVOID lpParam);	// worker thread for completion port	
	static unsigned int WINAPI PacketProcessThread(LPVOID lpParam);
	static unsigned int WINAPI MatchProcessThread(LPVOID lpParam);
	static unsigned int WINAPI inGameProcessThread(LPVOID lpParam);

	// for read cpu, memory usage
	static unsigned int WINAPI SystemUsageThread(LPVOID lpParam);

	static bool	ReceiveCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
	static bool	SendCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
	static bool	DisconnectCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
};

extern IOCPManager* g_pIocpManager;	// extern - for global freq
