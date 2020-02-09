#pragma once
#include "GameLogic.h"

class Entity
{
public:
	// Position : get / set	
	VECTOR3		GetPosition() { return m_vPos; }
	void		SetPosition(float _fPosX, float _fPosY, float _fPosZ);
	
	// Box Collider : get / set
	RECT		GetCharBox() { return m_rCharBox; }
	void		SetCharBox(float _fWidth, float _fHeight, bool _bCentered_Rect);
	void		SetCharBox(float _fWidth, float _fHeight, bool _bCentered_Rect, RECT _bDetail_Rect);

protected:
	VECTOR3		m_vPos;
	RECT		m_rCharBox;
};

