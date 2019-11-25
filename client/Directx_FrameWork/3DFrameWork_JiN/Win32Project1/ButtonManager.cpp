#include "ButtonManager.h"



CButtonManager::CButtonManager()
{
}


CButtonManager::~CButtonManager()
{
}

//bool 

void CButtonManager::Init(LPDIRECT3DDEVICE9 _pDevice)
{
	Button_GameStart.Init(_pDevice, L"2D_Sprites\\GameStart.png", 500.0f, 500.0f, 200.0f, 70.0f);
	//Button_Exit.Init(500.0f, 650.0f, 200.0f, 100.0f);
}

void CButtonManager::Update(float _fMouseX, float _fMouseY)
{
	if (Button_GameStart.On_Click(_fMouseX, _fMouseY))
	{

	}
}

void CButtonManager::Render()
{
	Button_GameStart.Render();
}

void CButtonManager::Clean()
{
	Button_GameStart.Clean();
}