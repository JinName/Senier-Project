#include "D3DSetup.h"


CD3DSetup::CD3DSetup()
	:m_pD3D(NULL),
	m_pd3dDevice(NULL),
	m_hWnd(NULL)
{
	
}


CD3DSetup::~CD3DSetup()
{
}

HRESULT CD3DSetup::InitD3D(HWND hWnd)
{
	m_hWnd = hWnd;

	// D3D ������Ʈ ����
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// D3DDevice ������ ���� ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// D3DDevice ����
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice)))
	{
		return E_FAIL;
	}

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting, since we are providing our own vertex colors
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���� ����̽� ������ �� ������
	OnInit(m_pd3dDevice);
	
	return S_OK;
}

VOID CD3DSetup::Shutdown()
{
}


void CD3DSetup::Update()
{
	// ������Ʈ �� ��
	OnUpdate(m_pd3dDevice);
}

void CD3DSetup::Render()
{
	RECT rt;
	rt.left = 0;
	rt.top = 0;
	rt.right = 64;
	rt.bottom = 64;
	// backbuffer �� �Ķ������� �ʱ�ȭ
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// ������ ����
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		//--------------------
		// ������ �Ұ�
		OnRender(m_pd3dDevice);
		//--------------------
				
		// ������ ��
		m_pd3dDevice->EndScene();
	}

	// ���
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void CD3DSetup::Cleanup()
{
	// �޸� ������ �� ��
	OnCleanup(m_pd3dDevice);

	if (m_pd3dDevice != NULL)
		m_pd3dDevice->Release();
	if (m_pD3D != NULL)
		m_pD3D->Release();
}