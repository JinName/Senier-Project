#include "WinSetup.h"
#include "Network.h"
#include "PacketManager.h"


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	// init
	CWinSetup *winSetup = new CWinSetup(hInst);	
	g_pNetwork = new Network();
	g_pPacketManager = new PacketManager();

	// run
	g_pNetwork->Run();
	winSetup->Run();
	
	// clean
	winSetup = NULL;
	delete winSetup;
	
	g_pPacketManager->SetStopFlag(true);

	delete g_pPacketManager;
	delete g_pNetwork;
	
	return 0;
}