#pragma once
#include "Header.h"
#include "Entity.h"
#include "Sprite.h"

// 번호에 따른 스타일 넘버
typedef enum {
	STYLE_LONG,
	STYLE_MID,
	STYLE_SHORT
} TILE_STYLE;

class CTile : public CEntity
{
	CSprite m_Sprite;

	bool m_bCollision_is_Possible;

	// 충돌한 상태면 true
	bool m_b_isCollision;

public:
	CTile();
	~CTile();

	bool Get_isCollision() { return m_b_isCollision; }
	void Set_isCollision(bool _isCollision) { m_b_isCollision = _isCollision; }

	bool Get_Collision_is_Possible() { return m_bCollision_is_Possible; }
	void Set_Collision_is_Possible(bool _bCollision_is_Possible) { m_bCollision_is_Possible = _bCollision_is_Possible; }

	void Init(LPDIRECT3DDEVICE9 _pDevice, TILE_STYLE _STYLE, D3DXVECTOR3 _vPos);
	void Update();
	void Render();
	void Clean();
};

