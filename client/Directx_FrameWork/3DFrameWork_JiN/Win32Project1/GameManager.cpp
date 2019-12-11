#include "GameManager.h"

CGameManager* g_pGameManager = NULL;

CGameManager::CGameManager()
{

}

CGameManager::CGameManager(HINSTANCE _hInstance, HWND _hWnd)
	:m_hWnd(_hWnd)
	,m_hInstance(_hInstance)
	,m_iPlayerIndex(-1)
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
		m_pGameBase->Set_PlayerIndex(m_iPlayerIndex);
		m_pGameBase->InitD3D(m_hWnd);		
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
		m_pGameBase->Set_PlayerIndex(m_iPlayerIndex);
		m_pGameBase->InitD3D(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == STAGE && m_pGameBase->Get_Change_Clear())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CGameClear;
		m_pGameBase->Set_PlayerIndex(m_iPlayerIndex);
		m_pGameBase->InitD3D(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == GAMEOVER && m_pGameBase->Get_Change_Restart())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CTitle;
		m_pGameBase->Set_PlayerIndex(m_iPlayerIndex);
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

bool CGameManager::GameStart(int _iPlayerIndex)
{
	if (m_pGameBase == NULL)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	m_iPlayerIndex = _iPlayerIndex;
	m_pGameBase->Set_GameStart(true);

	return true;
}

bool CGameManager::GameOver()
{
	if (m_pGameBase == NULL)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	m_iPlayerIndex = -1;
	m_pGameBase->Set_GameOver(true);

	return true;
}

bool CGameManager::SetPlayerState(SCHARACTER _sCharPacket)
{
	if (m_pGameBase == NULL)
	{
		cout << "GameBase is nullptr.." << endl;
		return false;
	}

	CStage* stage = (CStage*)m_pGameBase;
	CAru* player = stage->GetCharacter(_sCharPacket.mPlayerIndex);

	player->Set_Position(_sCharPacket.mPosX, _sCharPacket.mPosY, 0.0f);
	player->Set_Animation((int)_sCharPacket.mCharState);
}