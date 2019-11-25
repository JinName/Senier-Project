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

	// 빛의 반사로 색이 결정 된다.
	// r == 0 일땐 red 를 반사하지 않겠다. 라는 의미로 생각해야함.
	// ambient 변수는 재질이 반사시킬 빛의 밝기에 대한 설정
	// 오브젝트마다 재질을 바꾸고 싶다면 렌더 순서에 따라 setuplights() 를 하고 렌더하고를 반복
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
	// 라이트 타입에 따른 빛 종류 변경 가능
	light.Type = _lightType;
	light.Diffuse.r = 0.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	// 벡터 계산
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
		1.0f,
		sinf(timeGetTime() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = _lightLen;
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Finally, turn on some ambient light. - 밝기 조정 ( 얼마나 밝게 들어올 것인지 )
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	*/
}
