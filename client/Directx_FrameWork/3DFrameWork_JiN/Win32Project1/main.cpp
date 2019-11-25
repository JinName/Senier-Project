#include "WinSetup.h"


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	CWinSetup *winSetup = new CWinSetup(hInst);

	winSetup->Run();

	winSetup = NULL;
	delete winSetup;
	
	return 0;
}