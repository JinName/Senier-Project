#pragma once

#include "Header.h"

// ���� Ŭ����
// ������Ʈ���� SetMaterial �� ������ �޸����ִ°� ������ ����Ƽ�� ���̴� ���
// Material Ŭ������ �̿��� �پ��� ������ ����
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

