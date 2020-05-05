#pragma once
#include "Entity.h"

class Skill : public Entity
{
public:
	void SetDamage(int _iDamage) { m_iDamage = _iDamage; }
	int GetDamage() { return m_iDamage; }

private:
	int m_iDamage;
};

