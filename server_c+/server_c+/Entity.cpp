#include "Entity.h"

void Entity::SetPosition(float _fPosX, float _fPosY, float _fPosZ)
{
	m_vPos.x = _fPosX;
	m_vPos.y = _fPosY;
	m_vPos.z = _fPosZ;
}

void Entity::SetCollider(float _width, float _height, bool _centered_rect)
{
	if (_centered_rect)
	{
		m_rCollider.left = m_vPos.x - (_width / 2.0f);
		m_rCollider.right = m_vPos.x + (_width / 2.0f);
		m_rCollider.top = m_vPos.y - (_height / 2.0f);
		m_rCollider.bottom = m_vPos.y + (_height / 2.0f);
	}
	else
	{
		m_rCollider.left = m_vPos.x;
		m_rCollider.right = m_vPos.x + _width;
		m_rCollider.top = m_vPos.y;
		m_rCollider.bottom = m_vPos.y + _height;
	}
}

void Entity::SetCollider(float _width, float _height, bool _centered_rect, RECT _detail_rect)
{
	if (_centered_rect)
	{
		m_rCollider.left = m_vPos.x - (_width / 2.0f);
		m_rCollider.right = m_vPos.x + (_width / 2.0f);
		m_rCollider.top = m_vPos.y - (_height / 2.0f);
		m_rCollider.bottom = m_vPos.y + (_height / 2.0f);
	}
	else
	{
		m_rCollider.left = m_vPos.x;
		m_rCollider.right = m_vPos.x + _width;
		m_rCollider.top = m_vPos.y;
		m_rCollider.bottom = m_vPos.y + _height;
	}
	m_rCollider.left += _detail_rect.left;
	m_rCollider.right += _detail_rect.right;
	m_rCollider.top += _detail_rect.top;
	m_rCollider.bottom += _detail_rect.bottom;
}