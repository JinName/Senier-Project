#include "GameLogicManager.h"

GameLogicManager::GameLogicManager()
	:m_iPlayerCount(0), m_bInitComplete(false)
{
}

GameLogicManager::GameLogicManager(ClientSession* _client1, ClientSession* _client2)
	: m_iPlayerCount(0), m_bInitComplete(false)
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
	
	// FOR DEBUG
	//m_Player[0].SetDebugPlayer(true);

	m_bInitComplete = true;

	cout << "GameLogicManager > Init() > init complete!" << endl;
}

void GameLogicManager::Update()
{
	if (m_bInitComplete)
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

	player1.m_PlayerIndex = 0;
	player1.m_PosX = m_Player[0].GetPosition().x;
	player1.m_PosY = m_Player[0].GetPosition().y;

	player2.m_PlayerIndex = 1;
	player2.m_PosX = m_Player[1].GetPosition().x;
	player2.m_PosY = m_Player[1].GetPosition().y;

	SINGAMEINFO info(player1, player2);

	////////////
	bool result = false;
	result = g_pPacketManager->MakeSendPacket(m_Client[0], (char*)&info, sizeof(SINGAMEINFO), PROTOCOL::UPDATE_NF);
	m_Client[0]->Send();

	//result = PacketManager::GetInstance()->MakeSendPacket(m_Client[0], (char*)&player2, sizeof(SCHARACTER), PROTOCOL::UPDATE_NF);
	//m_Client[0]->Send();

	result = g_pPacketManager->MakeSendPacket(m_Client[1], (char*)&info, sizeof(SINGAMEINFO), PROTOCOL::UPDATE_NF);
	m_Client[1]->Send();

	//result = PacketManager::GetInstance()->MakeSendPacket(m_Client[1], (char*)&player2, sizeof(SCHARACTER), PROTOCOL::UPDATE_NF);
	//m_Client[1]->Send();
}