#include "Tile.h"

CTile::CTile()
{
}

CTile::~CTile()
{
}

void CTile::Init(LPDIRECT3DDEVICE9 _pDevice, TILE_STYLE _STYLE, D3DXVECTOR3 _vPos)
{
	m_bCollision_is_Possible = false;
	m_b_isCollision = false;
	//Line_Init(_pDevice);

	m_vPos = _vPos;

	switch(_STYLE)
	{
	case STYLE_LONG:
		m_Sprite.Create_Sprite(_pDevice, L"Objects\\LongTile.PNG", 433, 84, 1, NULL);
		Set_Collider(433, 84, false, RECT{ 8, 23, -8, 0 });
		break;

	case STYLE_MID:
		m_Sprite.Create_Sprite(_pDevice, L"Objects\\MidTile.PNG", 267, 79, 1, NULL);
		Set_Collider(267, 79, false, RECT{ 8, 23, -8, -5 });
		break;

	case STYLE_SHORT:
		m_Sprite.Create_Sprite(_pDevice, L"Objects\\ShortTile.PNG", 168, 76, 1, NULL);
		Set_Collider(168, 76, false, RECT{ 8, 23, -8, -5 });
		break;
	}
	m_Sprite.Set_Animation_Switch(false);
	m_Sprite.Set_Center_Rect(false);
}
void CTile::Update()
{

}

void CTile::Render()
{
	m_Sprite.DrawBitmap(&m_vPos, 0xFFFFFFFF, false);
	//Draw_RectLine();
}

void CTile::Clean()
{
	m_Sprite.CleanUp();
}