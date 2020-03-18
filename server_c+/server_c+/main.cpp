#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "IOCPManager.h"
#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"

int main()
{
	timeBeginPeriod(1);

	/// Global Managers initialize
	GLogger = new Logger;
	GSessionManager = new SessionManager;
	GIocpManager = new IOCPManager;

	GLogger->Init(LOGGER_TYPE::file);
	PacketManager::GetInstance()->Init();
	MatchManager::GetInstance()->Init();
	InGameManager::GetInstance()->Init();

	if (GIocpManager->InitIOCPServer() == false)
		return -1;

	if (GIocpManager->StartIOCPThread() == false)
		return -1;

	if (GIocpManager->StartPacketProcessThread() == false)
		return -1;

	if (GIocpManager->StartMatchProcessThread() == false)
		return -1;

	if (GIocpManager->StartInGameProcessThread() == false)
		return -1;

	cout << "Start IOCP Server..." << endl;

	if (GIocpManager->AcceptLoop() == false)
		return -1;

	GIocpManager->CloseIOCPServer();


	cout << "End IOCP Server..." << endl;

	MatchManager::GetInstance()->SetStopFlag(true);
	PacketManager::GetInstance()->SetStopFlag(true);

	// clean
	InGameManager::GetInstance()->Clean();
	MatchManager::GetInstance()->Clean();
	PacketManager::GetInstance()->Clean();

	// destroy singleton
	InGameManager::GetInstance()->DestroyInstance();
	MatchManager::GetInstance()->DestroyInstance();
	PacketManager::GetInstance()->DestroyInstance();
	delete GIocpManager;
	delete GSessionManager;
	delete GLogger;

	timeEndPeriod(1);

	return 0;
}