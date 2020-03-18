#pragma once
#include "GameLogic.h"
#include "PacketManager.h"
#include "Player.h"
#include "StageBase.h"
#include "StageForest.h"
#include "CollisionManager.h"

class GameLogicManager
{
public:
	GameLogicManager();
	GameLogicManager(ClientSession* _client1, ClientSession* _client2);
	~GameLogicManager();

	void Init(ClientSession* _client1, ClientSession* _client2);
	void Update();
	void Clean();

	Player* GetPlayer(int _iPlayerIndex) { return &m_Player[_iPlayerIndex]; }

	void SendPlayerState();

private:
	// Player 1, 2
	Player m_Player[2];
	ClientSession* m_Client[2];
	StageBase* m_StageBase;

	CollisionManager m_CollisionMgr;

	int m_iPlayerCount;

	bool m_bInitComplete;
};