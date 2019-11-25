#include "MapManager.h"


CMapManager::CMapManager()
{
}


CMapManager::~CMapManager()
{
}

void CMapManager::Create_Background(LPDIRECT3DDEVICE9 _pDevice, int _iStageNum)
{
	m_Background.Init(_pDevice);
}

void CMapManager::Create_Tiles(LPDIRECT3DDEVICE9 _pDevice, int _iStageNum)
{
	m_Tile[0].Init(_pDevice, STYLE_LONG, D3DXVECTOR3(-15.0f, 640.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[0].Get_Collider());

	m_Tile[1].Init(_pDevice, STYLE_SHORT, D3DXVECTOR3(470.0f, 540.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[1].Get_Collider());

	m_Tile[2].Init(_pDevice, STYLE_LONG, D3DXVECTOR3(720.0f, 590.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[2].Get_Collider());

	m_Tile[3].Init(_pDevice, STYLE_MID, D3DXVECTOR3(165.0f, 470.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[3].Get_Collider());
	
	m_Tile[4].Init(_pDevice, STYLE_LONG, D3DXVECTOR3(560.0f, 370.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[4].Get_Collider());

	m_Tile[5].Init(_pDevice, STYLE_MID, D3DXVECTOR3(650.0f, 100.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[5].Get_Collider());

	m_Tile[6].Init(_pDevice, STYLE_SHORT, D3DXVECTOR3(500.0f, 250.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[6].Get_Collider());

	m_Tile[7].Init(_pDevice, STYLE_LONG, D3DXVECTOR3(0.0f, 160.0f, 0.0f));
	m_Collider_List.push_back(m_Tile[7].Get_Collider());

	m_TileArray_Len = 8;
}

void CMapManager::StageMap_Init(LPDIRECT3DDEVICE9 _pDevice, int _iStageNum)
{
	Create_Background(_pDevice, _iStageNum);
	Create_Tiles(_pDevice, _iStageNum);
}

void CMapManager::StageMap_Update()
{

}

void CMapManager::StageMap_Render()
{
	m_Background.Render();

	for (int i = 0; i < 8; ++i)
	{
		m_Tile[i].Render();
	}
}

void CMapManager::StageMap_Cleanup()
{
	m_Background.Clean();

	for (int i = 0; i < 8; ++i)
	{
		m_Tile[i].Clean();
	}
}