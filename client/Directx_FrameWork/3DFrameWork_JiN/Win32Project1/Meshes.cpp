#include "Meshes.h"


CMeshes::CMeshes()
	:m_pMesh(NULL),
	m_pMeshMaterials(NULL),
	m_pMeshTextures(NULL),
	m_dwNumMaterials(0L)
{
}


CMeshes::~CMeshes()
{
}


HRESULT CMeshes::InitMeshes(char *_pFilename, LPDIRECT3DDEVICE9 _pDevice)
{
	// 임시 재질 버퍼
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// 파일이름으로 메시 읽어옴
	if (FAILED(D3DXLoadMeshFromX((LPCTSTR)_pFilename, D3DXMESH_SYSTEMMEM,
		_pDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
		&m_pMesh)))
	{
		// 없을 경우 상위 폴더 검색
		const CHAR* strPrefix = "..\\";
		CHAR strTexture[MAX_PATH];
		strcpy_s(strTexture, MAX_PATH, strPrefix);
		strcat_s(strTexture, MAX_PATH, _pFilename);
		if (FAILED(D3DXLoadMeshFromX((LPCTSTR)strTexture, D3DXMESH_SYSTEMMEM,
			_pDevice, NULL,
			&pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
			&m_pMesh)))
		{
			MessageBox(NULL, L"Could not find .x file", L"Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

	// 재질 정보와 텍스처 정보를 따로 뽑아냄
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	// 재질 정보만큼 재질의 구조체 배열 생성
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	if (m_pMeshMaterials == NULL)
		return E_OUTOFMEMORY;

	// 텍스처 정보만큼 텍스처의 구조체 배열 생성
	m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials]; 
	if (m_pMeshTextures == NULL)
		return E_OUTOFMEMORY;

	
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		// 재질 정보 복사
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

		m_pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFileA(_pDevice,
				d3dxMaterials[i].pTextureFilename,
				&m_pMeshTextures[i])))
			{
				// 파일이 없을 경우, 상위 폴더 검색
				const CHAR* strPrefix = "..\\";
				CHAR strTexture[MAX_PATH];
				strcpy_s(strTexture, MAX_PATH, strPrefix);
				strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				
				if (FAILED(D3DXCreateTextureFromFileA(_pDevice,
					strTexture,
					&m_pMeshTextures[i])))
				{
					MessageBox(NULL, L"Could not find texture map", L"Meshes.exe", MB_OK);
				}
			}
		}
	}

	// 임시 재질 버퍼 해제
	pD3DXMtrlBuffer->Release();

	return S_OK;
}
void CMeshes::Render(LPDIRECT3DDEVICE9 _pDevice)
{
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		// DrawSubset 을 위한 재질, 텍스처 셋팅
		// DrawSubset 은 폴리곤마다 다를 수 있는 재질과 텍스처 렌더링을 지원
		_pDevice->SetMaterial(&m_pMeshMaterials[i]);
		_pDevice->SetTexture(0, m_pMeshTextures[i]);

		// DrawSubset
		m_pMesh->DrawSubset(i);
	}
}

void CMeshes::Release()
{
	if(m_pMeshMaterials != NULL)
		delete[] m_pMeshMaterials;

	if (m_pMeshTextures)
	{
		for (DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			if (m_pMeshTextures[i])
				m_pMeshTextures[i]->Release();
		}
		delete[] m_pMeshTextures;
	}
	if (m_pMesh != NULL)
		m_pMesh->Release();
}