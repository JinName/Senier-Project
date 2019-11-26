#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "IOCPManager.h"
#include "PacketManager.h"
#include "MatchManager.h"

int main()
{
	/// Global Managers
	GSessionManager = new SessionManager;
	GIocpManager = new IOCPManager;
	PacketManager::GetInstance();
	MatchManager::GetInstance();


	if (GIocpManager->InitIOCPServer() == false)
		return -1;

	if (GIocpManager->StartIOCPThread() == false)
		return -1;

	if (GIocpManager->StartPacketProcessThread() == false)
		return -1;

	if (GIocpManager->StartMatchProcessThread() == false)
		return -1;

	cout << "Start IOCP Server..." << endl;

	if (GIocpManager->AcceptLoop() == false)
		return -1;

	GIocpManager->CloseIOCPServer();


	cout << "End IOCP Server..." << endl;

	MatchManager::DestroyInstance();
	PacketManager::DestroyInstance();
	delete GIocpManager;
	delete GSessionManager;

	return 0;
}