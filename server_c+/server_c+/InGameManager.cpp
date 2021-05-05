#include "InGameManager.h"

InGameManager::InGameManager() : m_RoomCount(0), m_LastRoomNum(0), m_IsStop(false)
{
	InitializeCriticalSection(&m_CS);
}

InGameManager::~InGameManager()
{
	DeleteCriticalSection(&m_CS);
}

void InGameManager::Init()
{
	m_RoomCount = 0;
	m_LastRoomNum = 0;

	InitializeCriticalSection(&m_CS);
}

void InGameManager::Clean()
{
	m_InGameRoomList.clear();

	DeleteCriticalSection(&m_CS);
}

bool InGameManager::InGame(ClientSession* player1, ClientSession* player2)
{
	if (player1 == nullptr || player2 == nullptr)
	{
		cout << "in Game Fail.." << endl;
		return false;
	}

	// into GameRoom
	InGameRoom* inGameRoom = new InGameRoom(player1, player2, m_LastRoomNum + 1);

	// init
	inGameRoom->Init();	

	m_InGameRoomList.push_back(inGameRoom);

	++m_LastRoomNum;
	++m_RoomCount;

	// send gamestart packet to client
	// setting player 1
	SGAMESTART player1_Gamestart;
	memset(&player1_Gamestart, 0, sizeof(SGAMESTART));
	player1_Gamestart.m_IsStart = true;
	player1_Gamestart.m_PlayerIndex = 0; // 1p
	player1_Gamestart.m_StartPosition[0] = inGameRoom->GetGameLogicManager()->GetPlayer(0)->GetPosition();
	player1_Gamestart.m_StartPosition[1] = inGameRoom->GetGameLogicManager()->GetPlayer(1)->GetPosition();
	player1->SetPlayerIndex(0);

	// setting player 2
	SGAMESTART player2_Gamestart;
	memset(&player2_Gamestart, 0, sizeof(SGAMESTART));
	player2_Gamestart.m_IsStart = true;
	player2_Gamestart.m_PlayerIndex = 1; // 2p
	player2_Gamestart.m_StartPosition[0] = inGameRoom->GetGameLogicManager()->GetPlayer(0)->GetPosition();
	player2_Gamestart.m_StartPosition[1] = inGameRoom->GetGameLogicManager()->GetPlayer(1)->GetPosition();
	player2->SetPlayerIndex(1);

	bool player1_result = g_pPacketManager->MakeSendPacket(player1, (char*)&player1_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);
	bool player2_result = g_pPacketManager->MakeSendPacket(player2, (char*)&player2_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);

	if (player1_result && player2_result)
	{
		// Send Game Start Packet
		player1->Send();
		player2->Send();
	}

	return true;
}

bool InGameManager::GameEnd(int roomNum, GAMEEND_STATE endState)
{
	if (roomNum > m_LastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	// 게임이 끝나는 방 번호로 찾음
	InGameRoom* room = SearchRoom(roomNum);

	// 게임 종료 원인을 패킷으로 전송
	SGAMEEND m_IsEnd;
	m_IsEnd.m_GameEndState = endState;

	g_pPacketManager->MakeSendPacket(room->GetClientSession(0), (char*)&m_IsEnd, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->GetClientSession(0)->Send();

	g_pPacketManager->MakeSendPacket(room->GetClientSession(1), (char*)&m_IsEnd, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->GetClientSession(1)->Send();

	return true;
}

bool InGameManager::OutGame(int roomNum)
{
	if (roomNum > m_LastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	InGameRoom* room = SearchRoom(roomNum);
	room->GetClientSession(0)->SetPlayerIndex(-1);
	room->GetClientSession(1)->SetPlayerIndex(-1);
	room->GetClientSession(0)->SetRoomNum(-1);
	room->GetClientSession(1)->SetRoomNum(-1);
	room->SetClientSession(0, nullptr);
	room->SetClientSession(1, nullptr);

	DeleteRoomInList(roomNum);

	// 안전하게 스레드 종료
	room->SetThreadStopFlag(true);
	room->Clean();

	if (room != nullptr)
		delete room;

	--m_RoomCount;

	return true;
}

InGameRoom* InGameManager::SearchRoom(int roomNum)
{
	if (roomNum > m_LastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return nullptr;
	}

	InGameRoom* room = nullptr;

	std::list<InGameRoom*>::iterator begin_iter = m_InGameRoomList.begin();
	std::list<InGameRoom*>::iterator end_iter = m_InGameRoomList.m_IsEnd();

	while (begin_iter != end_iter)
	{
		if ((*begin_iter)->GetRoomNum() == roomNum)
		{
			room = *begin_iter;
			break;
		}

		++begin_iter;
	}

	return room;
}

bool InGameManager::DeleteRoomInList(int roomNum)
{
	std::list<InGameRoom*>::iterator begin_iter = m_InGameRoomList.begin();
	std::list<InGameRoom*>::iterator end_iter = m_InGameRoomList.m_IsEnd();

	while (begin_iter != end_iter)
	{
		if ((*begin_iter)->GetRoomNum() == roomNum)
		{
			begin_iter = m_InGameRoomList.erase(begin_iter);

			break;
		}

		++begin_iter;
	}

	return true;
}

ClientSession* InGameManager::GetEnemyClient(ClientSession* player)
{
	if (player == nullptr)
	{
		cout << "player is nullptr.." << endl;
		return NULL;
	}

	ClientSession* enemyClient = nullptr;

	InGameRoom* room = SearchRoom(player->GetRoomNum());

	if (room == nullptr)
	{
		cout << "room is nullptr.." << endl;
		return NULL;
 	}

	if (room->GetClientSession(0) == nullptr || room->GetClientSession(1) == nullptr)
	{
		cout << "enemy client disconnected.." << endl;
		return NULL;
	}

	if (player == room->GetClientSession(0))
		enemyClient = room->GetClientSession(1);
	else if (player == room->GetClientSession(1))
		enemyClient = room->GetClientSession(0);

	return enemyClient;
}

SCHARACTER InGameManager::SetPlayer(ClientSession* player, SCHARACTER charPacket)
{
	InGameRoom* room = SearchRoom(player->GetRoomNum());

	room->SetPlayer(charPacket.m_PlayerIndex, charPacket);

	//charPacket.m_DirectionX = room->GetPlayerInfo(charPacket.m_PlayerIndex).GetDirection().x;
	//charPacket.m_PosX = room->GetPlayerInfo(charPacket.m_PlayerIndex).GetVector3().x;
	//charPacket.m_PosY = room->GetPlayerInfo(charPacket.m_PlayerIndex).GetVector3().y;

	return charPacket;
}

bool InGameManager::Enqueue(ClientPacket clientPacket)
{
	m_InGameBufferQueue.push(clientPacket);

	return true;
}

bool InGameManager::Dequeue()
{
	m_InGameBufferQueue.pop();

	return true;
}

void InGameManager::ProcessAllQueue()
{
	while (true)
	{
		if (m_IsStop) break;

		if (!m_InGameBufferQueue.empty())
		{
			// 먼저 처리되어야할 패킷을 꺼낸 후 삭제
			EnterCS();

			ClientPacket pack = m_InGameBufferQueue.front();
			m_InGameBufferQueue.pop();

			LeaveCS();

			// 패킷 헤드 확인
			PROTOCOL protocol = ParsingPacket(pack);

			// 프로토콜에 따른 패킷 처리
			ProcessPacket(protocol, pack);			
		}

		if (m_IsStop) break;
	}
}

PROTOCOL InGameManager::ParsingPacket(ClientPacket pack)
{
	if (pack.m_Buffer == nullptr)
	{
		cout << "parsing buffer is nullptr... return PROTOCOL::NONE" << endl;
		return PROTOCOL::NONE;
	}

	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	memcpy(&head, pack.m_Buffer, sizeof(SHEAD));

	return (PROTOCOL)head.m_Cmd;
}

void InGameManager::ProcessPacket(PROTOCOL protocol, ClientPacket pack)
{
	switch (protocol)
	{
	case PROTOCOL::MOVE_RQ:
	{
		SCHARACTER playerChar;
		memset(&playerChar, 0, sizeof(SCHARACTER));
		memcpy(&playerChar, pack.m_Buffer + sizeof(SHEAD), sizeof(SCHARACTER));

		InGameRoom* room = SearchRoom(pack.m_Session->GetRoomNum());

		room->SetPlayer(playerChar.m_PlayerIndex, playerChar);

		break;
	}

	case PROTOCOL::INITCOMPLETE_RQ:
	{
		SINITCOMPLETE sInit;
		memset(&sInit, 0, sizeof(SINITCOMPLETE));
		memcpy(&sInit, pack.m_Buffer + sizeof(SHEAD), sizeof(SINITCOMPLETE));

		InGameRoom* room = SearchRoom(pack.m_Session->GetRoomNum());

		room->SetInitComplete(sInit.m_PlayerIndex, sInit.m_IsComplete);

		if (room->GetInitComplete(0) == true && room->GetInitComplete(1) == true)
		{
			room->StartGameLogicThread();
		}

		break;
	}

	}
}