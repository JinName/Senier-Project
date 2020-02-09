#pragma once
#include "GameLogic.h"

class BoxCollider
{
public:
	BoxCollider();
	~BoxCollider();

	// init
	void Init();

	// RECT �浹
	bool isIntersect(RECT _object1, RECT _object2);

	// A�� B ���� �ִ��� üũ
	bool Player_upCheck(RECT _A, RECT _B);

	// ����, ���� �浹 ���� ��ȯ
	int Get_Vertical_or_Horizontal();

private:
	bool isVertical;
	bool isHorizontal;
};

