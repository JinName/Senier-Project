#include "GameManager.h"

CGameManager* g_pGameManager = nullptr;

CGameManager::CGameManager()
{

}

CGameManager::CGameManager(HINSTANCE _hInstance, HWND _hWnd)
	:m_hWnd(_hWnd)
	,m_hInstance(_hInstance)
	,m_PlayerIndex(-1)
{
	m_pGameBase = new CTitle;
	m_pGameBase->InitD3D(m_hWnd);
}

CGameManager::~CGameManager()
{
}

void CGameManager::Initialize()
{
	m_pGameBase = new CTitle;
	m_pGameBase->InitD3D(m_hWnd);

	// 카메라 초기화
	m_Camera.InitCamera(m_pGameBase->GetDevice());
}

void CGameManager::Update()
{
	m_pGameBase->Update();
	if (m_pGameBase->Get_GameScene_Num() == TITLE && m_pGameBase->Get_Change_GameStart())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CStage;
		m_pGameBase->Set_PlayerIndex(m_PlayerIndex);

		for (int i = 0; i < 2; ++i)
		{
			CAru* player = ((CStage*)m_pGameBase)->GetCharacter(i);

			player->Set_Position(m_vStartPosition[i].x, m_vStartPosition[i].y, 0.0f);

			if (i == 0)
				player->Set_Direction(1.0f, 0.0f);
			else if (i == 1)
				player->Set_Direction(-1.0f, 0.0f);
		}

		m_pGameBase->InitD3D(m_hWnd);		

		SINITCOMPLETE sInit;
		memset(&sInit, 0, sizeof(SINITCOMPLETE));
		sInit.m_PlayerIndex = m_PlayerIndex;
		sInit.m_IsComplete = true;
		g_pNetwork->SendPacket(PROTOCOL::INITCOMPLETE_RQ, (char*)&sInit, sizeof(SINITCOMPLETE), true);
	}
	else if (m_pGameBase->Get_GameScene_Num() == TITLE && m_pGameBase->Get_Exit())
	{
		DestroyWindow(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == STAGE && m_pGameBase->Get_Change_GameOver())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CGameOver;
		m_pGameBase->Set_PlayerIndex(m_PlayerIndex);
		m_pGameBase->InitD3D(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == STAGE && m_pGameBase->Get_Change_Clear())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CGameClear;
		m_pGameBase->Set_PlayerIndex(m_PlayerIndex);
		m_pGameBase->InitD3D(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == GAMEOVER && m_pGameBase->Get_Change_Restart())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CTitle;
		m_pGameBase->Set_PlayerIndex(m_PlayerIndex);
		m_pGameBase->InitD3D(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == GAMECLEAR && m_pGameBase->Get_Change_Restart())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CTitle;
		m_pGameBase->Set_PlayerIndex(m_PlayerIndex);
		m_pGameBase->InitD3D(m_hWnd);
	}
}

void CGameManager::Render()
{
	m_pGameBase->Render();
}

void CGameManager::Cleanup()
{
	m_pGameBase->Cleanup();
}

bool CGameManager::GameStart(int playerIndex)
{
	if (m_pGameBase == nullptr)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	m_PlayerIndex = playerIndex;
	m_pGameBase->Set_GameStart(true);

	return true;
}

bool CGameManager::GameOver()
{
	if (m_pGameBase == nullptr)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	m_PlayerIndex = -1;
	m_pGameBase->Set_GameOver(true);

	return true;
}

bool CGameManager::GameClear()
{
	if (m_pGameBase == nullptr)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	m_PlayerIndex = -1;
	m_pGameBase->Set_GameClear(true);

	return true;
}

bool CGameManager::SetPlayerState(SCHARACTER _sCharPacket)
{
	if (m_pGameBase == nullptr)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	CStage* stage = (CStage*)m_pGameBase;
	CAru* player = stage->GetCharacter(_sCharPacket.m_PlayerIndex);

	player->Set_Position(_sCharPacket.m_PosX, _sCharPacket.m_PosY, 0.0f);

	if (_sCharPacket.m_IsLeft == true)
	{
		player->Do_Left();
	}
	else if (_sCharPacket.m_IsRight == true)
	{
		player->Do_Right();
	}
	else if (_sCharPacket.m_IsLeft == false && _sCharPacket.m_IsRight == false)
	{
		player->Do_Stand();
	}
	
	if (_sCharPacket.m_IsAttack == true)
	{
		player->Do_Attack();
	}

	return true;
}

bool CGameManager::SetPlayerState(SCRASH _sCrash)
{
	if (m_pGameBase == nullptr)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	CStage* stage = (CStage*)m_pGameBase;
	CAru* player = stage->GetCharacter(_sCrash.m_PlayerIndex);

	player->Set_Active_Collision(false);

	return true;
}

bool CGameManager::SetPlayerPosition(SCHARACTER _sCharPacket)
{
	if (m_pGameBase == nullptr)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	CStage* stage = (CStage*)m_pGameBase;
	CAru* player = stage->GetCharacter(_sCharPacket.m_PlayerIndex);

	player->Set_Position(_sCharPacket.m_PosX, _sCharPacket.m_PosY, 0.0f);
}

void CGameManager::SetStartPosition(VECTOR3 _vPlayer1, VECTOR3 _vPlayer2)
{
	// player1
	m_vStartPosition[0].x = _vPlayer1.x;
	m_vStartPosition[0].y = _vPlayer1.y;
	m_vStartPosition[0].z = _vPlayer1.z;

	// player2
	m_vStartPosition[1].x = _vPlayer2.x;
	m_vStartPosition[1].y = _vPlayer2.y;
	m_vStartPosition[1].z = _vPlayer2.z;
}