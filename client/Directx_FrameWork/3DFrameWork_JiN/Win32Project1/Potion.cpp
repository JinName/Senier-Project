#include "Potion.h"

CPotion::CPotion()
{
}


CPotion::~CPotion()
{
}

void CPotion::Potion_Destroy()
{
	if (m_b_is_Collision == true)
	{
		m_bDestroy = true;
	}
}

void CPotion::Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _vPos)
{
	//Line_Init(_pDevice);

	m_vPos = _vPos;
	m_iRecoverPoint = 1;

	m_Sprite.Create_Sprite(_pDevice, L"2D_Sprites\\Potion.png", 640, 128, 5, D3DCOLOR_XRGB(0, 0, 0));
	Set_Collider(48.0f, 48.0f, true);
}

void CPotion::Update()
{
	m_Sprite.Animation_Frame();

	Potion_Destroy();
}

void CPotion::Render()
{
	m_Sprite.DrawBitmap(&m_vPos, 0xFFFFFFFF, false);

	//Draw_RectLine();
}

void CPotion::Clean()
{
	m_Sprite.CleanUp();
}