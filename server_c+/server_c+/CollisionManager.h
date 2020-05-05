#pragma once
#include "GameLogic.h"
#include "BoxCollider.h"
#include "Player.h"
#include "Tile.h"

class CollisionManager
{
public:
	CollisionManager() {}
	~CollisionManager() {}

	void Init();

	void Charater_Tile_Check(Tile* _Tile_Array, int _Tile_Array_Len, Player& _player);

private:
	BoxCollider m_boxCollider;
};