#include "Material.h"


CMaterial::CMaterial()
{
	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));
}

CMaterial::CMaterial(float _r, float _g, float _b, float _a)
{
	// 메모리 생성, 초기화
	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));

	m_mtrl.Diffuse.r = m_mtrl.Ambient.r = _r;
	m_mtrl.Diffuse.g = m_mtrl.Ambient.g = _r;
	m_mtrl.Diffuse.b = m_mtrl.Ambient.b = _r;
	m_mtrl.Diffuse.a = m_mtrl.Ambient.a = _r;
}


CMaterial::~CMaterial()
{
}

void CMaterial::SetDiffuseRGB(float _r, float _g, float _b, float _a)
{
	m_mtrl.Diffuse.r = _r;
	m_mtrl.Diffuse.g = _r;
	m_mtrl.Diffuse.b = _r;
	m_mtrl.Diffuse.a = _r;
}
void CMaterial::SetAmbientRGB(float _r, float _g, float _b, float _a)
{
	m_mtrl.Ambient.r = _r;
	m_mtrl.Ambient.g = _r;
	m_mtrl.Ambient.b = _r;
	m_mtrl.Ambient.a = _r;
}

