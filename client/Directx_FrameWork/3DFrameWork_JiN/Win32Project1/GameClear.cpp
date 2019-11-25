#include "GameClear.h"



CGameClear::CGameClear()
{
}

CGameClear::~CGameClear()
{
}

void CGameClear::OnInit(LPDIRECT3DDEVICE9 _pDevice)
{
	m_iGameScene_Num = GAMECLEAR;
	m_GameClear_Sprite.Create_Sprite(_pDevice, L"2D_Sprites\\GameClear.png", 318, 159, 1, D3DCOLOR_XRGB(255,255,255));
}

void CGameClear::OnUpdate(LPDIRECT3DDEVICE9 _pDevice)
{
}

void CGameClear::OnRender(LPDIRECT3DDEVICE9 _pDevice)
{
	m_GameClear_Sprite.DrawBitmap(&D3DXVECTOR3(490.0f, 370.0f, 0.0f), 0xFFFFFFFF, false);
}

void CGameClear::OnCleanup(LPDIRECT3DDEVICE9 _pDevice)
{
	m_GameClear_Sprite.CleanUp();
}