#pragma once
#include "Header.h"
#include "GameBase.h"
#include "Background.h"
#include "Aru.h"
#include "Tile.h"
#include "UIManager.h"
#include "MapManager.h"
#include "CollisionManager.h"
#include "Monster.h"
#include "Potion.h"

class CStage : public CGameBase
{
private:
	// 스테이지 넘버
	int m_iStageNum;

	CBackground m_Background;
	CAru m_Player;
	CMonster m_Monster[4];
	CPotion m_Potion[3];

	list<CMonster> m_Monster_List;
	list<CPotion> m_Potion_List;

	// UI 매니저
	CUIManager m_UIMnger;
	// 맵 매니저
	CMapManager m_MapMngr;
	// 충돌 처리 매니저
	CCollisionManager m_CollisionMngr;

	// 추락사 딜레이
	float m_fBunziHeight = 800;

public:
	CStage();
	~CStage();

	// GameClear Check
	bool GameClear();

	// GameOver Check
	bool GameOver();

	// Monster
	void Create_Monster(LPDIRECT3DDEVICE9 _pDevice);
	void Update_Monster();
	void Render_Monster();
	void Clean_Monster();

	// Potion
	void Create_Potion(LPDIRECT3DDEVICE9 _pDevice);
	void Update_Potion();
	void Render_Potion();
	void Clean_Potion();

	virtual void OnInit(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnUpdate(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnRender(LPDIRECT3DDEVICE9 _pDevice);
	virtual void OnCleanup(LPDIRECT3DDEVICE9 _pDevice);
};