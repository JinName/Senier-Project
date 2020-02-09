#pragma once
#include "GameLogic.h"
#include "Player.h"

class GameLogicManager
{
public:
	void Init();
	void Update();
	void Clean();

	Player* GetPlayer(int _iPlayerIndex) { return &m_Player[_iPlayerIndex]; }

private:
	// Player 1, 2
	Player m_Player[2];
};

