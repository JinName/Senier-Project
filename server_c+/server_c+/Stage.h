#pragma once
class Stage
{
public:
	void	SetMapNum(int _iMapNum) { m_iMapNum = _iMapNum; }
	int		GetMapNum() { return m_iMapNum; }

private:
	int		m_iMapNum;
};

