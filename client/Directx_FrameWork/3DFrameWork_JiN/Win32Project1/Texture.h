#pragma once
#include "Header.h"

class CTexture
{
protected:
	// �ؽ��� ������
	LPDIRECT3DTEXTURE9 m_pTexture;

public:
	CTexture();
	~CTexture();

	LPDIRECT3DTEXTURE9 Get_Texture() { return m_pTexture; }

	// ����̽��� �޾ƿ���, �̸��޾Ƽ� �ؽ�ó ���� �ε�
	HRESULT LoadTexture(LPDIRECT3DDEVICE9 _pDevice, std::wstring _filename, int _width, int _height, D3DCOLOR mask);
};

