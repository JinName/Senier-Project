#pragma once

#include "Header.h"
#include "Material.h"


class CLight
{
private:
	// 법선 벡터
	D3DXVECTOR3 vecDir;

	// 빛
	D3DLIGHT9 m_Light;

	// 빛의 길이

	// 빛의 종류
	D3DLIGHTTYPE m_lightType;

public:
	CLight();
	~CLight();

	// SetupLights(라이트 종류, 빛의 방향, 빛 길이)
	void SetupLights(D3DLIGHTTYPE _lightType, D3DXVECTOR3 _vecDir, float _lightLen);
};

