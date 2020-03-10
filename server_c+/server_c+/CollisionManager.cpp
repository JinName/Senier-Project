#include "CollisionManager.h"

void CollisionManager::Init()
{
	m_boxCollider.Init();
}

void CollisionManager::Charater_Tile_Check(Tile* _Tile_Array, int _Tile_Array_Len, Player& _player)
{
	int count = 0;

	for (int i = 0; i < _Tile_Array_Len; i++)
	{
		// �÷��̾ Ÿ�� ���� ������ Ÿ���� �浹 ���ɻ���
		if (m_boxCollider.Player_upCheck((&_player)->GetCollider(), _Tile_Array->GetCollider()))
		{
			_Tile_Array->Set_Collision_is_Possible(true);
			(&_player)->SetCollisionTile(_Tile_Array);
		}
		else
		{
			if (_Tile_Array->Get_isCollision() == false)
				_Tile_Array->Set_Collision_is_Possible(false);
		}

		if (_Tile_Array->Get_Collision_is_Possible() == true && (&_player)->Get_Collision_is_Possible() == true)
		{
			//cout << "[ TOP, LEFT : " << (&_player)->GetCollider().left << ", " << (&_player)->GetCollider().top << " ]" << endl;
			if (_Tile_Array == (&_player)->GetCollisionTile())
			{
				if (m_boxCollider.isIntersect(_Tile_Array->GetCollider(), (&_player)->GetCollider()))
				{
					// ���� , ���� �浹
					// 1 : ���� / 2 : ����
					if (m_boxCollider.Get_Vertical_or_Horizontal() == 1)
					{
						(&_player)->Set_isVertical(true);
						_Tile_Array->Set_isCollision(true);
					}
					++count;
				}
				else
				{
					_Tile_Array->Set_isCollision(false);
				}
			}
		}

		if (i == _Tile_Array_Len - 1)
		{
			if (count == 0)
			{
				(&_player)->Set_isVertical(false);
				(&_player)->Set_isHorizontal(false);
			}
		}		

		++_Tile_Array;
	}
}