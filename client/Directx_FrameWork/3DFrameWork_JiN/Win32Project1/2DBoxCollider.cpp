#include "2DBoxCollider.h"


C2DBoxCollider::C2DBoxCollider()
{
}

C2DBoxCollider::~C2DBoxCollider()
{
}

void C2DBoxCollider::Init()
{
	isVertical = false;
	isHorizontal = false;
}

bool C2DBoxCollider::isIntersect(RECT _A, RECT _B)
{
	if (_A.right >= _B.left && 
		_A.left <= _B.right && 
		_A.top <= _B.bottom && 
		_A.bottom >= _B.top)
	{
		// B�� A�� ������
		if (_A.left <= _B.left)
		{
			// B�� A�� ����
			if (_A.bottom >= _B.bottom)
			{
				if (std::labs(_A.right - _B.left) > std::labs(_B.bottom - _A.top))
				{
					isVertical = true;
				}
				else
				{
					isHorizontal = true;
				}
			}
			else // B�� A�� �Ʒ���
			{
				if (std::labs(_A.right - _B.left) > std::labs(_A.bottom - _B.top))
				{
					isVertical = true;
				}
				else
				{
					isHorizontal = true;
				}
			}
		}
		else // B�� A�� ����
		{
			// B�� A�� ����
			if (_A.bottom >= _B.bottom)
			{
				if (std::labs(_B.right - _A.left) > std::labs(_B.bottom - _A.top))
				{
					isVertical = true;
				}
				else
				{
					isHorizontal = true;
				}
			}
			else // B�� A�� �Ʒ���
			{
				if (std::labs(_B.right - _A.left) > std::labs(_A.bottom - _B.top))
				{
					isVertical = true;
				}
				else
				{
					isHorizontal = true;
				}
			}
		}
		return true;
	}

	isVertical = false;
	isHorizontal = false;

	return false;
}

bool C2DBoxCollider::Player_upCheck(RECT _A, RECT _B)
{
	if (_A.bottom < _B.top)
	{
		if (_A.left < _B.right && _A.right > _B.left)
		{
			return true;
		}
	}
	return false;
}

int C2DBoxCollider::Get_Vertical_or_Horizontal()
{
	if (isVertical == true && isHorizontal == false)
	{
		return 1;
	}
	else if (isVertical == false && isHorizontal == true)
	{
		return 2;
	}

	return 0;
}