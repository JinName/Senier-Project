#pragma once
#include "Header.h"
#include "GameBase.h"
#include "Camera.h"
#include "Stage.h"
#include "Title.h"
#include "GameOver.h"
#include "GameClear.h"
#include "PacketList.h"

class CGameManager
{
private:
	CGameBase *m_pGameBase;

	// Ä«¸Þ¶ó
	CCamera m_Camera;

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	int m_iPlayerIndex;
	
public:
	CGameManager();
	CGameManager(HINSTANCE _hInstance, HWND _hWnd);
	~CGameManager();

	void Initialize();

	bool GameStart(int _iPlayerIndex);
	bool GameOver();

	void Update();
	void Render();
	void Cleanup();

	bool SetPlayerState(SCHARACTER _sCharPacket);
};

extern CGameManager* g_pGameManager;

