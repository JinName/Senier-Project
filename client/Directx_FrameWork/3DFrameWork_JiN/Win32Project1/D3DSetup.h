#pragma once

#include "Header.h"

class CD3DSetup
{
protected:
	// ����̽� ������ ���� ����
	LPDIRECT3D9             m_pD3D;

	// ����̽� ������ ���� ����
	LPDIRECT3DDEVICE9       m_pd3dDevice;

	// ������ ����
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	// test texture
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPD3DXSPRITE m_pSprite;

protected:

	// �ݹ��Լ� - ȣ��Ǿ����� ó���ؾ��� �����ڵ带 ȣ���ؾ���
	// �������� ���, �������� �����Լ� ȣ��, ���� �����Լ�
	virtual void OnInit(LPDIRECT3DDEVICE9 _pDevice) = 0;
	virtual void OnUpdate(LPDIRECT3DDEVICE9 _pDevice) = 0;
	virtual void OnRender(LPDIRECT3DDEVICE9 _pDevice) = 0;
	virtual void OnCleanup(LPDIRECT3DDEVICE9 _pDevice) = 0;


public:
	CD3DSetup();
	CD3DSetup(LPDIRECT3DDEVICE9 _pDevice)
	{
		m_pd3dDevice = _pDevice;
	}
	~CD3DSetup();

	// ����̽� ��ȯ
	LPDIRECT3DDEVICE9 GetDevice() { return m_pd3dDevice; }
	HINSTANCE Get_hInstance() { return m_hInstance; }
	HWND Get_hWnd() { return m_hWnd; }

	// D3D �ʱ�ȭ
	HRESULT InitD3D(HWND hWnd);

	// D3D ����
	VOID Shutdown();

	// Update, Render, Cleanup
	void Update();
	void Render();
	void Cleanup();
};

