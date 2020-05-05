#pragma once
#include "GameLogic.h"
#include "Entity.h"

class Tile : public Entity
{
public:
	Tile() {}
	~Tile() {}

	void Init(VECTOR3 _vPos, float _fWidth, float _fHeight, bool _bCenteredRect, RECT _detailRect);

	bool Get_isCollision() { return m_b_isCollision; }
	void Set_isCollision(bool _isCollision) { m_b_isCollision = _isCollision; }

	bool Get_Collision_is_Possible() { return m_bCollision_is_Possible; }
	void Set_Collision_is_Possible(bool _bCollision_is_Possible) { m_bCollision_is_Possible = _bCollision_is_Possible; }

private:
	bool m_bCollision_is_Possible;

	// 충돌한 상태면 true
	bool m_b_isCollision;
};