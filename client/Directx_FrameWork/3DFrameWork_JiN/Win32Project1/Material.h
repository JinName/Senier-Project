#pragma once

#include "Header.h"

// 재질 클래스
// 오브젝트마다 SetMaterial 로 재질을 달리해주는게 렌더링 퀄리티를 높이는 방법
// Material 클래스를 이용해 다양한 재질을 관리
class CMaterial
{
private:
	D3DMATERIAL9 m_mtrl;

public:
	CMaterial();
	CMaterial(float _r, float _g, float _b, float _a);
	~CMaterial();

	void SetDiffuseRGB(float _r, float _g, float _b, float _a);
	void SetAmbientRGB(float _r, float _g, float _b, float _a);

	D3DMATERIAL9 GetMaterial() { return m_mtrl; }
};

