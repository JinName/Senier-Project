#include "GameLogicManager.h"

GameLogicManager::GameLogicManager()
	:m_iPlayerCount(0)
{
}

GameLogicManager::GameLogicManager(ClientSession* _client1, ClientSession* _client2)
	: m_iPlayerCount(0)
{
	m_Client[0] = _client1;
	m_Client[1] = _client2;
}

GameLogicManager::~GameLogicManager()
{
}

void GameLogicManager::Init(ClientSession* _client1, ClientSession* _client2)
{
	m_Client[0] = _client1;
	m_Client[1] = _client2;

	m_StageBase = new StageForest();

	m_StageBase->Init();

	for (int i = 0; i < 2; ++i)
	{
		m_Player[i].Init(i);
		++m_iPlayerCount;
	}
}

void GameLogicManager::Update()
{
	for (int i = 0; i < 2; ++i)
	{
		m_Player[i].Update();
	}

	// 충돌체크 다른 업데이트 끝난 후 마지막에
	for (int i = 0; i < 2; ++i)
	{
		m_CollisionMgr.Charater_Tile_Check(m_StageBase->GetTileArray(), m_StageBase->GetTileCount(), m_Player[i]);
	}
	
	SendPlayerState();
}

void GameLogicManager::Clean()
{
	if (m_StageBase != NULL)
		delete m_StageBase;
}

void GameLogicManager::SendPlayerState()
{
	// setting packet
	SCHARACTER player1;
	SCHARACTER player2;
	memset(&player1, 0, sizeof(SCHARACTER));
	memset(&player2, 0, sizeof(SCHARACTER));

	player1.mPlayerIndex = 0;
	player1.mPosX = m_Player[0].GetPosition().x;
	player1.mPosY = m_Player[0].GetPosition().y;

	player2.mPlayerIndex = 1;
	player2.mPosX = m_Player[1].GetPosition().x;
	player2.mPosY = m_Player[1].GetPosition().y;

	////////////
	bool result = false;
	result = PacketManager::GetInstance()->MakeSendPacket(m_Client[0], (char*)&player1, sizeof(SCHARACTER), PROTOCOL::UPDATE_NF);
	m_Client[0]->Send();

	result = PacketManager::GetInstance()->MakeSendPacket(m_Client[0], (char*)&player2, sizeof(SCHARACTER), PROTOCOL::UPDATE_NF);
	m_Client[0]->Send();

	result = PacketManager::GetInstance()->MakeSendPacket(m_Client[1], (char*)&player1, sizeof(SCHARACTER), PROTOCOL::UPDATE_NF);
	m_Client[1]->Send();

	result = PacketManager::GetInstance()->MakeSendPacket(m_Client[1], (char*)&player2, sizeof(SCHARACTER), PROTOCOL::UPDATE_NF);
	m_Client[1]->Send();
}