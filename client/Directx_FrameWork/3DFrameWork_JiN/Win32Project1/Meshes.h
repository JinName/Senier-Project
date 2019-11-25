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

	// ���� �̸����� �޽� �а� �ʱ�ȭ
	HRESULT InitMeshes(char *_pFilename, LPDIRECT3DDEVICE9 _pDevice);
	
	// �޽� ������
	void Render(LPDIRECT3DDEVICE9 _pDevice);

	// �޽� ����
	void Release();
};

