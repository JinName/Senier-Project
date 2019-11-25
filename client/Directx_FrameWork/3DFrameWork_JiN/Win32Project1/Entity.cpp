#include "Entity.h"



CEntity::CEntity()
{
}

CEntity::~CEntity()
{
}

void CEntity::Set_Collider(float _width, float _height, bool _centered_rect)
{
	if (_centered_rect)
	{
		m_rt.left = m_vPos.x - (_width / 2.0f);
		m_rt.right = m_vPos.x + (_width / 2.0f);
		m_rt.top = m_vPos.y - (_height / 2.0f);
		m_rt.bottom = m_vPos.y + (_height / 2.0f);
	}
	else
	{
		m_rt.left = m_vPos.x;
		m_rt.right = m_vPos.x + _width;
		m_rt.top = m_vPos.y;
		m_rt.bottom = m_vPos.y + _height;
	}

	vList[0] = D3DXVECTOR2(m_rt.left, m_rt.top);
	vList[1] = D3DXVECTOR2(m_rt.right, m_rt.top);
	vList[2] = D3DXVECTOR2(m_rt.right, m_rt.bottom);
	vList[3] = D3DXVECTOR2(m_rt.left, m_rt.bottom);
	vList[4] = D3DXVECTOR2(m_rt.left, m_rt.top);
}

void CEntity::Set_Collider(float _width, float _height, bool _centered_rect, RECT _detail_rect)
{
	if (_centered_rect)
	{
		m_rt.left = m_vPos.x - (_width / 2.0f);
		m_rt.right = m_vPos.x + (_width / 2.0f);
		m_rt.top = m_vPos.y - (_height / 2.0f);
		m_rt.bottom = m_vPos.y + (_height / 2.0f);
	}
	else
	{
		m_rt.left = m_vPos.x;
		m_rt.right = m_vPos.x + _width;
		m_rt.top = m_vPos.y;
		m_rt.bottom = m_vPos.y + _height;
	}
	m_rt.left += _detail_rect.left;
	m_rt.right += _detail_rect.right;
	m_rt.top += _detail_rect.top;
	m_rt.bottom += _detail_rect.bottom;

	vList[0] = D3DXVECTOR2(m_rt.left, m_rt.top);
	vList[1] = D3DXVECTOR2(m_rt.right, m_rt.top);
	vList[2] = D3DXVECTOR2(m_rt.right, m_rt.bottom);
	vList[3] = D3DXVECTOR2(m_rt.left, m_rt.bottom);
	vList[4] = D3DXVECTOR2(m_rt.left, m_rt.top);
}

void CEntity::Line_Init(LPDIRECT3DDEVICE9 _pDevice)
{
	D3DXCreateLine(_pDevice, &m_pLine);
	m_pLine->SetWidth(2);
}

void CEntity::Draw_RectLine()
{
	m_pLine->Begin();
	m_pLine->Draw(vList, 5, D3DCOLOR_XRGB(255, 0, 0));
	m_pLine->End();
}