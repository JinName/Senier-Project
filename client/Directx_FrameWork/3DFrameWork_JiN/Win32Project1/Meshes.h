#pragma once
#include "Header.h"

class CMeshes
{
private:
	LPD3DXMESH          m_pMesh = NULL;				// Our mesh object in sysmem
	D3DMATERIAL9*       m_pMeshMaterials = NULL;	// Materials for our mesh
	LPDIRECT3DTEXTURE9* m_pMeshTextures = NULL;		// Textures for our mesh
	DWORD               m_dwNumMaterials = 0L;		// Number of mesh materials
public:
	CMeshes();
	~CMeshes();

	// 파일 이름으로 메시 읽고 초기화
	HRESULT InitMeshes(char *_pFilename, LPDIRECT3DDEVICE9 _pDevice);
	
	// 메시 렌더링
	void Render(LPDIRECT3DDEVICE9 _pDevice);

	// 메시 해제
	void Release();
};

