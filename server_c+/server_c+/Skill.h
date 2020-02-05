#pragma once
class Skill
{
public:
	void SetDamage(int _iDamage) { m_iDamage = _iDamage; }
	int GetDamage() { return m_iDamage; }

private:
	int m_iDamage;
};

