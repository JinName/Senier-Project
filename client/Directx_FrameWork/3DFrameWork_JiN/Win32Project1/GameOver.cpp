#include "GameOver.h"



CGameOver::CGameOver()
{
}


CGameOver::~CGameOver()
{
}

void CGameOver::OnInit(LPDIRECT3DDEVICE9 _pDevice)
{
	m_iGameScene_Num = GAMEOVER;

	m_GameOver_Sprite.Create_Sprite(_pDevice, L"2D_Sprites\\GameOver.png", 426, 240, 1, NULL);
}

void CGameOver::OnUpdate(LPDIRECT3DDEVICE9 _pDevice)
{
	if (CInput::Get_Instance()->IsKeyPressed(DIK_SPACE))
	{
		m_bRestart = true;
	}
}

void CGameOver::OnRender(LPDIRECT3DDEVICE9 _pDevice)
{
	m_GameOver_Sprite.DrawBitmap(&D3DXVECTOR3(490.0f, 370.0f, 0.0f), 0xFFFFFFFF, false);
}

void CGameOver::OnCleanup(LPDIRECT3DDEVICE9 _pDevice)
{
	m_GameOver_Sprite.CleanUp();
}