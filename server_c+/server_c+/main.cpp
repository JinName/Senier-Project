#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "IOCPManager.h"

int main()
{
	/// Global Managers
	GSessionManager = new SessionManager;
	GIocpManager = new IOCPManager;


	if (GIocpManager->InitIOCPServer() == false)
		return -1;

	if (GIocpManager->StartIOCPThread() == false)
		return -1;

	cout << "Start IOCP Server..." << endl;

	if (GIocpManager->AcceptLoop() == false)
		return -1;

	GIocpManager->CloseIOCPServer();


	cout << "End IOCP Server..." << endl;

	delete GIocpManager;
	delete GSessionManager;

	return 0;
}