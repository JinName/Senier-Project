#pragma once

#include "Header.h"
#include "Material.h"


class CLight
{
private:
	// ���� ����
	D3DXVECTOR3 vecDir;

	// ��
	D3DLIGHT9 m_Light;

	// ���� ����

	// ���� ����
	D3DLIGHTTYPE m_lightType;

public:
	CLight();
	~CLight();

	// SetupLights(����Ʈ ����, ���� ����, �� ����)
	void SetupLights(D3DLIGHTTYPE _lightType, D3DXVECTOR3 _vecDir, float _lightLen);
};

