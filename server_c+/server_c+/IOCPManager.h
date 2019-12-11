#pragma once
#include "stdafx.h"
#include "SessionManager.h"
#include "ClientSession.h"
#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"

class IOCPManager
{
public:
	IOCPManager();
	~IOCPManager();

	bool		InitIOCPServer();	// initialize
	bool		StartIOCPThread();	// start iocp worker thread
	bool		StartPacketProcessThread();	// start packet process thread
	bool		StartMatchProcessThread();	// start match process thread
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

	static bool	ReceiveCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
	static bool	SendCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
	static bool	DisconnectCompletion(ClientSession* client, SOVERLAPPED* overlapped, DWORD dwBytesTransferred);
};

extern IOCPManager* GIocpManager;	// extern - for global freq
