#include "Camera.h"


CCamera::CCamera()
{
}


CCamera::~CCamera()
{
}

void CCamera::InitCamera(LPDIRECT3DDEVICE9 _pMainDevice)
{
	m_pCameraDevice = _pMainDevice;

	m_vEyePt = D3DXVECTOR3(0.0f, 3.0f, -5.0f);
	m_vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	D3DXMatrixLookAtLH(&m_mtViw, &m_vEyePt, &m_vLookatPt, &m_vUpVec);
	m_pCameraDevice->SetTransform(D3DTS_VIEW, &m_mtViw);


	// D3DXMatrixPerspectiveFovLH() �Լ���
	// �޼� ��ǥ��(LH: Left Hand Coordinate System),
	// ���� ����(Perspective), �þ� ��(FOV: Field Of View),
	// ȭ�� ��Ⱦ ��(Aspect Ratio: Screen Width/ Screen Height),
	// ���� ȭ���� �ٰŸ�(����� ��: Near),
	// ���� ȭ���� ���Ÿ�(Far)�� �μ��� ��
	//D3DXMatrixPerspectiveFovLH(&m_mtPrj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	D3DXMatrixOrthoLH(&m_mtPrj, 800, 600, NULL, NULL);
	m_pCameraDevice->SetTransform(D3DTS_PROJECTION, &m_mtPrj);
}

void CCamera::CameraMove()
{
	
}