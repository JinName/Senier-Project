#include "GameLogicManager.h"


void GameLogicManager::Init()
{
	for (int i = 0; i < 2; ++i)
		m_Player[i].Init(i);
}

void GameLogicManager::Update()
{
}

void GameLogicManager::Clean()
{
}