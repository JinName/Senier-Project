#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "IOCPManager.h"
#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"
#include "GameDBManager.h"
#include "Crypt.h"
#include "SystemUsage.h"

int main()
{
	timeBeginPeriod(1);

	// system usage
	g_pSystemUsage = new SystemUsage;
	g_pSystemUsage->Init();

	/// Global Managers initialize
	g_pConnLogger = new Logger;
	g_pUsageLogger = new Logger;
	g_pCrypt = new Crypt;
	g_pSessionManager = new SessionManager;
	g_pIocpManager = new IOCPManager;
	g_pGameDBManager = new GameDBManager;

	g_pConnLogger->Init(LOGGER_TYPE::file, "connect_log");
	g_pUsageLogger->Init(LOGGER_TYPE::file, "usage_log");
	g_pCrypt->Init();
	g_pGameDBManager->Init();
	PacketManager::GetInstance()->Init();
	MatchManager::GetInstance()->Init();
	InGameManager::GetInstance()->Init();

	g_pGameDBManager->Connect();

	if (g_pIocpManager->InitIOCPServer() == false)
		return -1;

	if (g_pIocpManager->StartIOCPThread() == false)
		return -1;

	if (g_pIocpManager->StartPacketProcessThread() == false)
		return -1;

	if (g_pIocpManager->StartMatchProcessThread() == false)
		return -1;

	if (g_pIocpManager->StartSystemUsageThread() == false)
		return -1;

	cout << "Start IOCP Server..." << endl;

	if (g_pIocpManager->AcceptLoop() == false)
		return -1;

	g_pIocpManager->CloseIOCPServer();

	cout << "End IOCP Server..." << endl;

	MatchManager::GetInstance()->SetStopFlag(true);
	PacketManager::GetInstance()->SetStopFlag(true);

	// clean
	InGameManager::GetInstance()->Clean();
	MatchManager::GetInstance()->Clean();
	PacketManager::GetInstance()->Clean();

	g_pGameDBManager->Disconnect();
	g_pCrypt->Clean();

	// destroy singleton
	InGameManager::GetInstance()->DestroyInstance();
	MatchManager::GetInstance()->DestroyInstance();
	PacketManager::GetInstance()->DestroyInstance();
	delete g_pIocpManager;
	delete g_pSessionManager;
	delete g_pCrypt;
	delete g_pConnLogger;
	delete g_pUsageLogger;
	delete g_pSystemUsage;

	timeEndPeriod(1);

	return 0;
}