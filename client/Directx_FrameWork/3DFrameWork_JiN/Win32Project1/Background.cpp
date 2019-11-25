#include "Background.h"



CBackground::CBackground()
{
}


CBackground::~CBackground()
{
}

void CBackground::Init(LPDIRECT3DDEVICE9 _pDevice)
{
	m_vPos = { 0.0f, 0.0f, 0.0f };

	m_Sprite.Create_Sprite(_pDevice, L"Objects\\Background2.jpg", 960, 720, 1, NULL);
	m_Sprite.Set_Animation_Switch(false);
	m_Sprite.Set_Center_Rect(false);
}

void CBackground::Update()
{
	// ������Ʈ �� �� ����
}

void CBackground::Render()
{
	m_Sprite.DrawBitmap(&m_vPos, 0xFFFFFFFF, NULL);
}

void CBackground::Clean()
{
	m_Sprite.CleanUp();
}