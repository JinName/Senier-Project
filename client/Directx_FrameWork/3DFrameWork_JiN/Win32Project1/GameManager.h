#pragma once
#include "Header.h"
#include "GameBase.h"
#include "Camera.h"
#include "Stage.h"
#include "Title.h"
#include "GameOver.h"
#include "GameClear.h"

class CGameManager
{
private:
	CGameBase *m_pGameBase;

	// Ä«¸Þ¶ó
	CCamera m_Camera;

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	
public:
	CGameManager();
	CGameManager(HINSTANCE _hInstance, HWND _hWnd);
	~CGameManager();

	void Initialize();

	void Update();
	void Render();
	void Cleanup();
};

