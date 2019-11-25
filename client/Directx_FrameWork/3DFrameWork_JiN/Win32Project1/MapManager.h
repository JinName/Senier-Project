#pragma once
#include "Header.h"
#include "Background.h"
#include "Tile.h"

// 타일 생성
class CMapManager
{
private:
	// 타일 충돌박스 리스트
	list<RECT> m_Collider_List;

	// 맵 구성 객체들
	CBackground m_Background;
	CTile m_Tile[8];
	int m_TileArray_Len;

	list<CTile> m_TileList;

	// 뒷 배경 생성
	void Create_Background(LPDIRECT3DDEVICE9 _pDevice, int _iStageNum);
	// 맵 바닥 생성
	void Create_Tiles(LPDIRECT3DDEVICE9 _pDevice, int _iStageNum);
public:
	CMapManager();
	~CMapManager();

	void StageMap_Init(LPDIRECT3DDEVICE9 _pDeviceint, int _iStageNum);
	void StageMap_Update();
	void StageMap_Render();
	void StageMap_Cleanup();

	// 타일리스트 반환
	CTile* Get_TileArray() { return m_Tile; }
	int Get_TileArray_Len() { return m_TileArray_Len; }
	list<CTile> Get_TileList() { return m_TileList; }
	// 충돌박스 리스트 반환
	list<RECT> Get_Collider_List() { return m_Collider_List; }
};

