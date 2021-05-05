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
	HANDLE		GetCPHandle()		{ return m_CP; }
	int			GetThreadCount()	{ return m_ThreadCount; }

private:
	HANDLE		m_CP;				// handle - for completion port
	int			m_ThreadCount;		// count of workerThread = sysinfo.dwNumberOfProcessors * 2

	SOCKET		m_ListenSocket;		// socket - for listen

	// static function - must private.. not to public
	static unsigned int WINAPI workerThread(LPVOID lpParam);	// worker thread for completion port	
	static unsigned int WINAPI packetProcessThread(LPVOID lpParam);
	static unsigned int WINAPI matchProcessThread(LPVOID lpParam);
	static unsigned int WINAPI inGameProcessThread(LPVOID lpParam);

	// for read cpu, memory usage
	static unsigned int WINAPI systemUsageThread(LPVOID lpParam);

	static bool	receiveCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
	static bool	sendCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
	static bool	disconnectCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
};

extern IOCPManager* g_pIocpManager;	// extern - for global freq
