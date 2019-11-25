#pragma once
#include "Header.h"

class CTexture
{
protected:
	// 텍스쳐 포인터
	LPDIRECT3DTEXTURE9 m_pTexture;

public:
	CTexture();
	~CTexture();

	LPDIRECT3DTEXTURE9 Get_Texture() { return m_pTexture; }

	// 디바이스는 받아오고, 이름받아서 텍스처 정보 로드
	HRESULT LoadTexture(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filename, int _width, int _height, D3DCOLOR mask);
};

