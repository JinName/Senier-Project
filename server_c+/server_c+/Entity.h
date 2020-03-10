#pragma once
#include "GameLogic.h"

class Entity
{
public:
	Entity() {}
	~Entity() {}

	// Position : get / set	
	VECTOR3		GetPosition() { return m_vPos; }
	void		SetPosition(float _fPosX, float _fPosY, float _fPosZ);
	
	// Box Collider : get / set
	RECT		GetCollider() { return m_rCollider; }
	void		SetCollider(float _fWidth, float _fHeight, bool _bCentered_Rect);
	void		SetCollider(float _fWidth, float _fHeight, bool _bCentered_Rect, RECT _bDetail_Rect);

protected:
	VECTOR3		m_vPos;
	RECT		m_rCollider;
};

