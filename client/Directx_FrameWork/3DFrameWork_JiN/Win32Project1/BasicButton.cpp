#include "BasicButton.h"



CBasicButton::CBasicButton()
{
}


CBasicButton::~CBasicButton()
{
}

bool CBasicButton::On_Click(float _fMouseX, float _fMouseY)
{
	if (_fMouseX > m_rtButton.left && _fMouseX < m_rtButton.right &&
		_fMouseY > m_rtButton.top && _fMouseY < m_rtButton.bottom)
	{
		return true;
	}

	return false;
}

void CBasicButton::Init(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filename, float _fPosX, float _fPosY, float _fButtonWidth, float _fButtonHeight)
{
	m_rtButton.left = _fPosX;
	m_rtButton.top = _fPosY;
	m_rtButton.right = _fPosX + _fButtonWidth;
	m_rtButton.bottom = _fPosY + _fButtonHeight;

	m_fButtonWidth = _fButtonWidth;
	m_fButtonHeight = _fButtonHeight;

	m_Sprite.Create_Sprite(_pDevice, _filename.c_str(), m_fButtonWidth, m_fButtonHeight, 1, NULL);
	m_Sprite.Set_Center_Rect(false);
}
void CBasicButton::Update()
{

}
void CBasicButton::Render()
{
	m_Sprite.DrawBitmap(&D3DXVECTOR3(m_rtButton.left, m_rtButton.top, 0), 0xFFFFFFFF, false);
}
void CBasicButton::Clean()
{
	m_Sprite.CleanUp();
}