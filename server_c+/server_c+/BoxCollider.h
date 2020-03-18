#pragma once
#include "GameLogic.h"

enum class CollisionType
{
	NONE, VERTICAL, HORIZONTAL
};

class BoxCollider
{
public:
	BoxCollider();
	~BoxCollider();

	// init
	void Init();

	// RECT 충돌
	CollisionType isIntersect(RECT _object1, RECT _object2);

	// A가 B 위에 있는지 체크
	bool Player_upCheck(RECT _A, RECT _B);

	// 수직, 수평 충돌 여부 반환
	int Get_Vertical_or_Horizontal();

private:

};