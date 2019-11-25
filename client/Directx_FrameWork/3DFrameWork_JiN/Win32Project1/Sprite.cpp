#include "Sprite.h"



CSprite::CSprite()
	:m_pSprite(NULL)
	,m_dwSpriteTime(100)
	,m_iWidth(0)
	,m_iHeight(0)
	,m_iSprite_Count(0)
	,m_rectWidth(0)
	,m_rectHeight(0)
	,m_bAnimation_Switch(true)
	,m_bCenter_Rect(true)
	,m_bLast_Frame(false)
{
	m_SpriteRect = { 0, 0, 0, 0 };
}


CSprite::~CSprite()
{
}

HRESULT CSprite::Create_Sprite(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filePath, int _width, int _height, int _spriteCount, D3DCOLOR mask)
{
	//m_pDevice = _pDevice;
	m_iWidth = _width;
	m_iHeight = _height;
	m_iSprite_Count = _spriteCount;

	m_rectWidth = m_iWidth / m_iSprite_Count;
	m_rectHeight = m_iHeight;

	m_SpriteRect.left = 0;
	m_SpriteRect.top = 0;
	m_SpriteRect.right = m_rectWidth;
	m_SpriteRect.bottom = m_rectHeight;

	m_vCenter = { (float)(m_rectWidth/2), (float)(m_rectHeight/2), 0 };

	m_dwOldTime = GetTickCount();

	LoadTexture(_pDevice, _filePath.c_str(), _width, _height, mask);

	D3DXCreateSprite(_pDevice, &m_pSprite);

	return S_OK;
}


void CSprite::DrawBitmap(D3DXVECTOR3* pos, D3DCOLOR mask, bool reverse)
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	// flip the sprite horizontally
	
	D3DXMATRIX oldMat;
	m_pSprite->GetTransform(&oldMat);
	if (reverse)
	{
		D3DXMATRIX mat;

		D3DXVECTOR2 scale(-1.0f, 1.0f);		// flip horizon
		D3DXVECTOR2 center(pos->x, pos->y);		// set flip center(upper left corner of the sprite)
		D3DXVECTOR2 translate(0, 0);

		D3DXMatrixTransformation2D(&mat, &center, NULL, &scale, NULL, NULL, &translate);
		m_pSprite->SetTransform(&mat);
	}

	if(m_bCenter_Rect)
		m_pSprite->Draw(m_pTexture, &m_SpriteRect, &m_vCenter, pos, mask);
	else
		m_pSprite->Draw(m_pTexture, &m_SpriteRect, NULL, pos, mask);

	m_pSprite->SetTransform(&oldMat);
	m_pSprite->End();
}

void CSprite::Animation_Frame()
{
	if (m_bAnimation_Switch == true)
	{
		DWORD dwCurrentTime = GetTickCount();

		if (dwCurrentTime - m_dwOldTime >= m_dwSpriteTime)
		{
			m_bLast_Frame = false; // 마지막 프레임일때 true

			m_dwOldTime = dwCurrentTime;
			m_SpriteRect.left += m_rectWidth;
			m_SpriteRect.right += m_rectWidth;

			if (m_SpriteRect.right == m_iWidth)
			{
				m_bLast_Frame = true; // 마지막 프레임
			}
			if (m_SpriteRect.left >= m_iWidth)
			{
				m_SpriteRect.left = 0;
				m_SpriteRect.right = m_rectWidth;
			}
		}
	}
}

void CSprite::Select_Sprite(int _Sprite_Num)
{
	m_SpriteRect.left = _Sprite_Num * m_rectWidth;
	m_SpriteRect.right = m_SpriteRect.left + m_rectWidth;

	m_SpriteRect.top = 0;
	m_SpriteRect.bottom = m_SpriteRect.top + m_rectHeight;
}

void CSprite::Reset_Sprite()
{
	m_SpriteRect.left = 0;
	m_SpriteRect.right = m_rectWidth;
}

void CSprite::CleanUp()
{
	if (m_pSprite != NULL)
	{
		m_pSprite->Release();
		m_pTexture->Release();
	}
}