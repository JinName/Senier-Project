#include "Tile.h"

void Tile::Init(VECTOR3 _vPos, float _fWidth, float _fHeight, bool _bCenteredRect, RECT _detailRect)
{
	// Tile 초기설정
	m_bCollision_is_Possible = false;
	m_b_isCollision = false;

	m_vPos = _vPos;

	SetCollider(_fWidth, _fHeight, false, _detailRect);
}