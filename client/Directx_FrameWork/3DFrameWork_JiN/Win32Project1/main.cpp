#include "WinSetup.h"
#include "Network.h"
#include "PacketManager.h"


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	CWinSetup *winSetup = new CWinSetup(hInst);
	
	Network::GetInstance()->Run();
	PacketManager::GetInstance();
	winSetup->Run();
	
	winSetup = NULL;
	delete winSetup;
	
	PacketManager::GetInstance()->Clean();
	PacketManager::GetInstance()->DestroyInstance();
	Network::GetInstance()->DestroyInstance();	
	
	return 0;
}