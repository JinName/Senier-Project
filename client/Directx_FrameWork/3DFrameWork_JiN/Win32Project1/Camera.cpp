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


	// D3DXMatrixPerspectiveFovLH() 함수는
	// 왼손 좌표계(LH: Left Hand Coordinate System),
	// 투시 투영(Perspective), 시야 각(FOV: Field Of View),
	// 화면 종횡 비(Aspect Ratio: Screen Width/ Screen Height),
	// 가상 화면의 근거리(가까운 면: Near),
	// 가상 화면의 원거리(Far)을 인수로 함
	//D3DXMatrixPerspectiveFovLH(&m_mtPrj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	D3DXMatrixOrthoLH(&m_mtPrj, 800, 600, NULL, NULL);
	m_pCameraDevice->SetTransform(D3DTS_PROJECTION, &m_mtPrj);
}

void CCamera::CameraMove()
{
	
}