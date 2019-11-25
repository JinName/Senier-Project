#include "Texture.h"



CTexture::CTexture()
	:m_pTexture(NULL)
{
}


CTexture::~CTexture()
{
	/*
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		//delete m_pTexture;
	}
	*/
}

HRESULT CTexture::LoadTexture(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filename, int _width, int _height, D3DCOLOR mask)
{
	if (FAILED(D3DXCreateTextureFromFileEx(_pDevice, _filename.c_str()
		, _width // 파일의 폭
		, _height // 파일의 높이
		, 1 // 밉맵 2d : 1 / 3d : default
		, 0
		, D3DFMT_UNKNOWN // 파일 포맷
		, D3DPOOL_MANAGED // 메모리 저장될 곳 지정
		, D3DX_DEFAULT // 필터
		, D3DX_DEFAULT // 밉필터
		, mask // 컬러 키
		, NULL
		, NULL
		, &m_pTexture))) // output
	{
		MessageBox(NULL, L"Could not find...", L"LoadTextures.exe", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}