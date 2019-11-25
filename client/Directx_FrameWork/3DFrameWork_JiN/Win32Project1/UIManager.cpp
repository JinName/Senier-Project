#include "UIManager.h"



CUIManager::CUIManager()
{
}


CUIManager::~CUIManager()
{
}

void CUIManager::Init(LPDIRECT3DDEVICE9 _pDevice, int _Player_HP)
{
	m_Health.Init(_pDevice, _Player_HP, D3DXVECTOR3(50.0f, 50.0f, 0.0f));
}

void CUIManager::Update(int _Player_HP)
{
	m_Health.Set_HP(_Player_HP);
	m_Health.Update();
}

void CUIManager::Render()
{
	m_Health.Render();
}

void CUIManager::Clean()
{
	m_Health.Clean();
}