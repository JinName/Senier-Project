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
		, _width // ������ ��
		, _height // ������ ����
		, 1 // �Ӹ� 2d : 1 / 3d : default
		, 0
		, D3DFMT_UNKNOWN // ���� ����
		, D3DPOOL_MANAGED // �޸� ����� �� ����
		, D3DX_DEFAULT // ����
		, D3DX_DEFAULT // ������
		, mask // �÷� Ű
		, NULL
		, NULL
		, &m_pTexture))) // output
	{
		MessageBox(NULL, L"Could not find...", L"LoadTextures.exe", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}