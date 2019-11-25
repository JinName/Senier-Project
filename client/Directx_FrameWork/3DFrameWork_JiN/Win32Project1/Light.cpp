#include "Light.h"



CLight::CLight()
{
}


CLight::~CLight()
{
}

void CLight::SetupLights(D3DLIGHTTYPE _lightType, D3DXVECTOR3 _vecDir, float _lightLen)
{
	/*
	// Set up a material. The material here just has the diffuse and ambient
	// colors set to yellow. Note that only one material can be used at a time.

	// ���� �ݻ�� ���� ���� �ȴ�.
	// r == 0 �϶� red �� �ݻ����� �ʰڴ�. ��� �ǹ̷� �����ؾ���.
	// ambient ������ ������ �ݻ��ų ���� ��⿡ ���� ����
	// ������Ʈ���� ������ �ٲٰ� �ʹٸ� ���� ������ ���� setuplights() �� �ϰ� �����ϰ� �ݺ�
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	//g_pd3dDevice->SetMaterial(&mtrl);

	// Set up a white, directional light, with an oscillating direction.
	// Note that many Lights may be active at a time (but each one slows down
	// the rendering of our scene). However, here we are just using one. Also,
	// we need to set the D3DRS_LIGHTING renderstate to enable lighting
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	// ����Ʈ Ÿ�Կ� ���� �� ���� ���� ����
	light.Type = _lightType;
	light.Diffuse.r = 0.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	// ���� ���
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
		1.0f,
		sinf(timeGetTime() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = _lightLen;
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Finally, turn on some ambient light. - ��� ���� ( �󸶳� ��� ���� ������ )
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	*/
}
