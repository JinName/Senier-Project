#include "WinSetup.h"
#include "Network.h"
#include "PacketManager.h"


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	// init
	//CWinSetup *winSetup = new CWinSetup(hInst);	
	g_pWinSetup = new CWinSetup(hInst);
	g_pNetwork = new Network();
	g_pPacketManager = new PacketManager();

	// run
	g_pNetwork->Run();
	g_pWinSetup->Run();
	
	// clean
	delete g_pWinSetup;
	
	g_pPacketManager->SetStopFlag(true);

	delete g_pPacketManager;
	delete g_pNetwork;
	
	return 0;
}