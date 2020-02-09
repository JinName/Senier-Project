#include "Entity.h"

void Entity::SetPosition(float _fPosX, float _fPosY, float _fPosZ)
{
	m_vPos.x = _fPosX;
	m_vPos.y = _fPosY;
	m_vPos.z = _fPosZ;
}