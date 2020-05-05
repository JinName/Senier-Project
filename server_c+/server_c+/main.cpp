#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "IOCPManager.h"
#include "PacketManager.h"
#include "MatchManager.h"
#include "InGameManager.h"
#include "GameDBManger.h"

int main()
{
	timeBeginPeriod(1);

	/// Global Managers initialize
	g_pLogger = new Logger;
	g_pSessionManager = new SessionManager;
	g_pIocpManager = new IOCPManager;
	g_pGameDBManager = new GameDBManger;

	g_pLogger->Init(LOGGER_TYPE::file);
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

	//if (g_pIocpManager->StartInGameProcessThread() == false)
	//	return -1;

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

	// destroy singleton
	InGameManager::GetInstance()->DestroyInstance();
	MatchManager::GetInstance()->DestroyInstance();
	PacketManager::GetInstance()->DestroyInstance();
	delete g_pIocpManager;
	delete g_pSessionManager;
	delete g_pLogger;

	timeEndPeriod(1);

	return 0;
}