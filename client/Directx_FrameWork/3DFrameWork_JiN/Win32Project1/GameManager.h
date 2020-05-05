#pragma once
#include "Header.h"
#include "GameBase.h"
#include "Camera.h"
#include "Stage.h"
#include "Title.h"
#include "GameOver.h"
#include "GameClear.h"
#include "PacketList.h"
#include "Network.h"

class CGameManager
{
private:
	CGameBase *m_pGameBase;

	// Ä«¸Þ¶ó
	CCamera m_Camera;

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	int m_iPlayerIndex;
	D3DXVECTOR3 m_vStartPosition[2];
	
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
	bool SetPlayerPosition(SCHARACTER _sCharPacket);
	void SetStartPosition(VECTOR3 _vPlayer1, VECTOR3 _Player2);
};

extern CGameManager* g_pGameManager;

