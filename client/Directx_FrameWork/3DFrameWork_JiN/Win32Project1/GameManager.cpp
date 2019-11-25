#include "GameManager.h"



CGameManager::CGameManager()
{

}

CGameManager::CGameManager(HINSTANCE _hInstance, HWND _hWnd)
	:m_hWnd(_hWnd)
	,m_hInstance(_hInstance)
{
	m_pGameBase = new CTitle;
	m_pGameBase->InitD3D(m_hWnd);
}

CGameManager::~CGameManager()
{
}

void CGameManager::Initialize()
{
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
		m_pGameBase->InitD3D(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == STAGE && m_pGameBase->Get_Change_Clear())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CGameClear;
		m_pGameBase->InitD3D(m_hWnd);
	}
	else if (m_pGameBase->Get_GameScene_Num() == GAMEOVER && m_pGameBase->Get_Change_Restart())
	{
		m_pGameBase->Cleanup();
		delete m_pGameBase;

		m_pGameBase = new CStage;
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