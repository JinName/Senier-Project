#pragma once
#include "stdafx.h"

class IOCPManager
{
private:
	HANDLE		mCP;				// handle - for completion port
	int			mThreadCount;		// count of WorkerThread = sysinfo.dwNumberOfProcessors * 2

	SOCKET		mListenSocket;		// socket - for listen

	// static function - must private.. not to public
	static unsigned int WINAPI WorkerThread(LPVOID lpParam);	// worker thread for completion port

public:
	IOCPManager();
	~IOCPManager();

	bool		InitIOCPServer();	// initialize
	bool		StartIOCPThread();	// start iocp worker thread
	bool		CloseIOCPServer();	// close iocp server	

	bool		AcceptLoop();		// accept loop

	// get
	HANDLE		GetCPHandle()		{ return mCP; }
	int			GetThreadCount()	{ return mThreadCount; }
};

extern IOCPManager* GIocpManager;	// extern - for global freq
