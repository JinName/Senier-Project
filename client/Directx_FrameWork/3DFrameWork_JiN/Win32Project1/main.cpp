#include "WinSetup.h"
#include "Network.h"


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	CWinSetup *winSetup = new CWinSetup(hInst);
	
	Network::GetInstance()->Run();

	winSetup->Run();

	winSetup = NULL;
	delete winSetup;
	
	Network::GetInstance()->DestroyInstance();
	
	return 0;
}