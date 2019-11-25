#include "GameBase.h"

CGameBase::CGameBase()
	:m_bGameStart(false)
	,m_bGameOver(false)
	,m_bGameClear(false)
	,m_bRestart(false)
	,m_bExit(false)
{

}

void CGameBase::OnInit(LPDIRECT3DDEVICE9 _pDevice)
{
}

void CGameBase::OnUpdate(LPDIRECT3DDEVICE9 _pDevice)
{
}

void CGameBase::OnRender(LPDIRECT3DDEVICE9 _pDevice)
{
}

void CGameBase::OnCleanup(LPDIRECT3DDEVICE9 _pDevice)
{
}

void CGameBase::SetPosition(float _fx, float _fy, float _fz)
{
	m_pos.m_fx = _fx;
	m_pos.m_fy = _fy;
	m_pos.m_fz = _fz;

	//matrix 연산 필요

	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, m_pos.m_fx, m_pos.m_fy, m_pos.m_fz);
	//D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
}