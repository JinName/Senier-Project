#include "CollisionManager.h"



CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Init()
{
	m_boxCollider.Init();
}

/*
void CCollisionManager::Update(list<RECT> &_Rect_List, CAru &_aru)
{
	std::list<RECT>::iterator begin_iter = _Rect_List.begin();
	std::list<RECT>::iterator end_iter = _Rect_List.end();
	int count = 0;

	for (int i = 0; i < _Rect_List.size(); i++)
	{
		if (m_boxCollider.isIntersect(*begin_iter, (&_aru)->Get_Collider()))
		{
			// 수직 , 수평 충돌
			// 1 : 수직 / 2 : 수평
			if (m_boxCollider.Get_Vertical_or_Horizontal() == 1)
			{
				(&_aru)->Set_isVertical(true);
			}
			else if (m_boxCollider.Get_Vertical_or_Horizontal() == 2)
			{
				//(&_aru)->Set_isHorizontal(true);
			}
			count++;
		}

		begin_iter++;
		
		if (begin_iter == end_iter)
		{
			if (count == 0)
			{
				(&_aru)->Set_isVertical(false);
				(&_aru)->Set_isHorizontal(false);
			}
		}
	}
}
*/

void CCollisionManager::Charater_Tile_Check(CTile* _Tile_Array, int _Tile_Array_Len, CAru &_aru)
{
	int count = 0;
	for(int i = 0; i < _Tile_Array_Len; i++)
	{
		if (_Tile_Array->Get_Collision_is_Possible() == true && (&_aru)->Get_Collision_is_Possible() == true)
		{
			if (m_boxCollider.isIntersect(_Tile_Array->Get_Collider(), (&_aru)->Get_Collider()))
			{
				// 수직 , 수평 충돌
				// 1 : 수직 / 2 : 수평
				if (m_boxCollider.Get_Vertical_or_Horizontal() == 1)
				{
					(&_aru)->Set_isVertical(true);
					_Tile_Array->Set_isCollision(true);
				}
				++count;
			}
			else
			{
				_Tile_Array->Set_isCollision(false);
			}
			
		}

		if (i == _Tile_Array_Len - 1)
		{
			if (count == 0)
			{
				(&_aru)->Set_isVertical(false);
				(&_aru)->Set_isHorizontal(false);
			}
		}

		// 플레이어가 타일 위에 있으면 타일이 충돌 가능상태
		if (m_boxCollider.Player_upCheck((&_aru)->Get_Collider(), _Tile_Array->Get_Collider()))
		{
			_Tile_Array->Set_Collision_is_Possible(true);
		}
		else
		{
			if (_Tile_Array->Get_isCollision() == false)
				_Tile_Array->Set_Collision_is_Possible(false);
		}

		++_Tile_Array;
	}
}

void CCollisionManager::CharAttack_Monster_Check(list<CFireBall*> &_FireBall_List, list<CMonster> &_Monster_List)
{
	std::list<CFireBall*>::iterator fire_begin_iter = _FireBall_List.begin();
	std::list<CFireBall*>::iterator fire_end_iter = _FireBall_List.end();

	std::list<CMonster>::iterator Monster_begin_iter = _Monster_List.begin();
	std::list<CMonster>::iterator Monster_end_iter = _Monster_List.end();

	while (Monster_begin_iter != Monster_end_iter)
	{
		while (fire_begin_iter != fire_end_iter)
		{
			if ((*fire_begin_iter)->Get_Collision_is_Possible())
			{
				if (m_boxCollider.isIntersect(Monster_begin_iter->Get_Collider(), (*fire_begin_iter)->Get_Collider()))
				{
					Monster_begin_iter->Set_isCollision(true);
					if ((*fire_begin_iter)->Get_Position().x < Monster_begin_iter->Get_Position().x)
						Monster_begin_iter->Set_Collision_Direction(-1);
					else if ((*fire_begin_iter)->Get_Position().x > Monster_begin_iter->Get_Position().x)
						Monster_begin_iter->Set_Collision_Direction(1);

					(*fire_begin_iter)->Set_isCollision(true);
				}
			}
			++fire_begin_iter;
		}
		fire_begin_iter = _FireBall_List.begin();
		++Monster_begin_iter;
	}
}

void CCollisionManager::Charater_Monster_Check(CAru &_aru, list<CMonster> &_Monster_List)
{
	std::list<CMonster>::iterator Monster_begin_iter = _Monster_List.begin();
	std::list<CMonster>::iterator Monster_end_iter = _Monster_List.end();

	if ((&_aru)->Get_Active_Collision())
	{
		while (Monster_begin_iter != Monster_end_iter)
		{
			if (m_boxCollider.isIntersect(Monster_begin_iter->Get_Collider(), (&_aru)->Get_Collider()))
			{
				// 플레이어 충돌
				(&_aru)->Set_Active_Collision(false);
			}
			++Monster_begin_iter;
		}
	}
}

void CCollisionManager::Charater_Potion_Check(CAru &_aru, list<CPotion> &_Potion_List)
{
	std::list<CPotion>::iterator Potion_begin_iter = _Potion_List.begin();
	std::list<CPotion>::iterator Potion_end_iter = _Potion_List.end();

	if ((&_aru)->Get_HP_isFull() == false)
	{
		while (Potion_begin_iter != Potion_end_iter)
		{
			if (m_boxCollider.isIntersect(Potion_begin_iter->Get_Collider(), (&_aru)->Get_Collider()))
			{
				// 플레이어 체력 회복 / 포션 소멸
				(&_aru)->Set_Potion_Collision(true);
				Potion_begin_iter->Set_isCollision(true);
			}
			++Potion_begin_iter;
		}
	}
}