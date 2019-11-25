#pragma once

#include "Header.h"
#include "WinConfigure.h"

class CCamera
{
private:
	LPDIRECT3DDEVICE9	m_pCameraDevice;	// CameraDevice = MainDevice

	D3DXMATRIX          m_mtViw;            // View Matrix
	D3DXMATRIX          m_mtPrj;            // Projection Matrix

	D3DXVECTOR3			m_vEyePt;			// Camera Position
	D3DXVECTOR3			m_vLookatPt;		// Look Vector
	D3DXVECTOR3			m_vUpVec;			// Up Vector
public:
	CCamera();
	~CCamera();

	// ī�޶� ������ �ʱ�ȭ
	void InitCamera(LPDIRECT3DDEVICE9 _pMainDevice);

	// ī�޶� ��ġ ����
	void CameraMove();
};

