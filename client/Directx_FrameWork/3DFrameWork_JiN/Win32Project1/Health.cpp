#include "Health.h"



CHealth::CHealth()
{
}


CHealth::~CHealth()
{
}

void CHealth::Init(LPDIRECT3DDEVICE9 _pDevice, int _iHP, D3DXVECTOR3 _vPos)
{
	m_iHP = _iHP;
	m_iHP_Texture_Count = _iHP;
	m_vPos = _vPos;
	// Player HP Texture
	for (int i = 0; i < m_iHP_Texture_Count; ++i)
	{
		m_Sprite[i].Create_Sprite(_pDevice, L"2D_Sprites\\HP_Sprite.png", 100, 50, 2, NULL);
		m_Sprite[i].Set_Animation_Switch(false);
	}
}

void CHealth::Update()
{
	if (m_iHP == 3)
	{
		m_Sprite[0].Select_Sprite(0);
		m_Sprite[1].Select_Sprite(0);
		m_Sprite[2].Select_Sprite(0);
	}
	else if (m_iHP == 2)
	{
		m_Sprite[0].Select_Sprite(0);
		m_Sprite[1].Select_Sprite(0);
		m_Sprite[2].Select_Sprite(1);
	}
	else if (m_iHP == 1)
	{
		m_Sprite[0].Select_Sprite(0);
		m_Sprite[1].Select_Sprite(1);
		m_Sprite[2].Select_Sprite(1);
	}
	else if (m_iHP == 0)
	{
		m_Sprite[0].Select_Sprite(1);
		m_Sprite[1].Select_Sprite(1);
		m_Sprite[2].Select_Sprite(1);
	}
}

void CHealth::Render()
{
	for (int i = 0; i < m_iHP_Texture_Count; ++i)
	{
		m_Sprite[i].DrawBitmap(&(m_vPos + D3DXVECTOR3((float)i * 52.0f, 0.0f, 0.0f)), 0xFFFFFFFF, false);
	}
}

void CHealth::Clean()
{
	for (int i = 0; i < m_iHP_Texture_Count; ++i)
	{
		m_Sprite[i].CleanUp();
	}
}