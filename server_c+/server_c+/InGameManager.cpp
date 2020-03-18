#include "InGameManager.h"

InGameManager::InGameManager() : mRoomCount(0), mLastRoomNum(0), mStopFlag(false)
{
	InitializeCriticalSection(&mCS);
}

InGameManager::~InGameManager()
{
	DeleteCriticalSection(&mCS);
}

void InGameManager::Init()
{
	mRoomCount = 0;
	mLastRoomNum = 0;

	InitializeCriticalSection(&mCS);
}

void InGameManager::Clean()
{
	mInGameRoomList.clear();

	DeleteCriticalSection(&mCS);
}

bool InGameManager::InGame(ClientSession* player1, ClientSession* player2)
{
	if (player1 == nullptr || player2 == nullptr)
	{
		cout << "in Game Fail.." << endl;
		return false;
	}

	// into GameRoom
	InGameRoom* inGameRoom = new InGameRoom(player1, player2, mLastRoomNum + 1);

	// init
	inGameRoom->Init();	

	mInGameRoomList.push_back(inGameRoom);

	++mLastRoomNum;
	++mRoomCount;

	// send gamestart packet to client
	// setting player 1
	SGAMESTART player1_Gamestart;
	memset(&player1_Gamestart, 0, sizeof(SGAMESTART));
	player1_Gamestart.mStart = true;
	player1_Gamestart.mPlayerIndex = 0; // 1p
	player1_Gamestart.mStartPosition[0] = inGameRoom->GetGameLogicManager()->GetPlayer(0)->GetPosition();
	player1_Gamestart.mStartPosition[1] = inGameRoom->GetGameLogicManager()->GetPlayer(1)->GetPosition();
	player1->SetPlayerIndex(0);

	// setting player 2
	SGAMESTART player2_Gamestart;
	memset(&player2_Gamestart, 0, sizeof(SGAMESTART));
	player2_Gamestart.mStart = true;
	player2_Gamestart.mPlayerIndex = 1; // 2p
	player2_Gamestart.mStartPosition[0] = inGameRoom->GetGameLogicManager()->GetPlayer(0)->GetPosition();
	player2_Gamestart.mStartPosition[1] = inGameRoom->GetGameLogicManager()->GetPlayer(1)->GetPosition();
	player2->SetPlayerIndex(1);

	bool player1_result = PacketManager::GetInstance()->MakeSendPacket(player1, (char*)&player1_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);
	bool player2_result = PacketManager::GetInstance()->MakeSendPacket(player2, (char*)&player2_Gamestart, sizeof(SGAMESTART), PROTOCOL::GAMESTART_CM);

	if (player1_result && player2_result)
	{
		// Send Game Start Packet
		player1->Send();
		player2->Send();
	}

	//inGameRoom->StartGameLogicThread();

	return true;
}

bool InGameManager::GameEnd(int roomNum, GAMEEND_STATE endState)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	// 게임이 끝나는 방 번호로 찾음
	InGameRoom* room = SearchRoom(roomNum);

	// 게임 종료 원인을 패킷으로 전송
	SGAMEEND end;
	end.mGameEndState = endState;

	PacketManager::GetInstance()->MakeSendPacket(room->GetClientSession(0), (char*)&end, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->GetClientSession(0)->Send();

	PacketManager::GetInstance()->MakeSendPacket(room->GetClientSession(1), (char*)&end, sizeof(SGAMEEND), PROTOCOL::GAMEEND_CM);
	room->GetClientSession(1)->Send();

	return true;
}

bool InGameManager::OutGame(int roomNum)
{
	if (roomNum > mLastRoomNum)
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

	--mRoomCount;

	return true;
}

InGameRoom* InGameManager::SearchRoom(int roomNum)
{
	if (roomNum > mLastRoomNum)
	{
		cout << "wrong room number.." << endl;
		return false;
	}

	InGameRoom* room = nullptr;

	std::list<InGameRoom*>::iterator begin_iter = mInGameRoomList.begin();
	std::list<InGameRoom*>::iterator end_iter = mInGameRoomList.end();

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
	std::list<InGameRoom*>::iterator begin_iter = mInGameRoomList.begin();
	std::list<InGameRoom*>::iterator end_iter = mInGameRoomList.end();

	while (begin_iter != end_iter)
	{
		if ((*begin_iter)->GetRoomNum() == roomNum)
		{
			begin_iter = mInGameRoomList.erase(begin_iter);

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

	room->SetPlayer(charPacket.mPlayerIndex, charPacket);

	//charPacket.mDirectionX = room->GetPlayerInfo(charPacket.mPlayerIndex).GetDirection().x;
	//charPacket.mPosX = room->GetPlayerInfo(charPacket.mPlayerIndex).GetVector3().x;
	//charPacket.mPosY = room->GetPlayerInfo(charPacket.mPlayerIndex).GetVector3().y;

	return charPacket;
}

bool InGameManager::Enqueue(ClientPacket clientPacket)
{
	mInGameBufferQueue.push(clientPacket);

	return true;
}

bool InGameManager::Dequeue()
{
	mInGameBufferQueue.pop();

	return true;
}

void InGameManager::ProcessAllQueue()
{
	while (true)
	{
		if (mStopFlag) break;

		if (!mInGameBufferQueue.empty())
		{
			// 먼저 처리되어야할 패킷을 꺼낸 후 삭제
			EnterCS();

			ClientPacket pack = mInGameBufferQueue.front();
			mInGameBufferQueue.pop();

			// 패킷 헤드 확인
			PROTOCOL protocol = ParsingPacket(pack);

			// 프로토콜에 따른 패킷 처리
			ProcessPacket(protocol, pack);

			LeaveCS();
		}

		if (mStopFlag) break;
	}
}

PROTOCOL InGameManager::ParsingPacket(ClientPacket pack)
{
	if (pack.mBuffer == nullptr)
	{
		cout << "parsing buffer is nullptr... return PROTOCOL::NONE" << endl;
		return PROTOCOL::NONE;
	}

	SHEAD head;
	memset(&head, 0, sizeof(SHEAD));
	memcpy(&head, pack.mBuffer, sizeof(SHEAD));

	return (PROTOCOL)head.mCmd;
}

void InGameManager::ProcessPacket(PROTOCOL protocol, ClientPacket pack)
{
	switch (protocol)
	{
	case PROTOCOL::MOVE_RQ:
	{
		SCHARACTER playerChar;
		memset(&playerChar, 0, sizeof(SCHARACTER));
		memcpy(&playerChar, pack.mBuffer + sizeof(SHEAD), sizeof(SCHARACTER));

		InGameRoom* room = SearchRoom(pack.mSession->GetRoomNum());

		room->SetPlayer(playerChar.mPlayerIndex, playerChar);

		// 연산 처리 후 클라이언트에 플레이어 현재 정보에 대한 패킷 전송
		//playerChar.mPosX = room->GetGameLogicManager()->GetPlayer(playerChar.mPlayerIndex)->GetPosition().x;
		//playerChar.mPosY = room->GetGameLogicManager()->GetPlayer(playerChar.mPlayerIndex)->GetPosition().y;
		//PacketManager::GetInstance()->MakeSendPacket(room->GetClientSession(0), (char*)&playerChar, sizeof(SCHARACTER), PROTOCOL::BRCAST_MOVE_RP);
		//PacketManager::GetInstance()->MakeSendPacket(room->GetClientSession(1), (char*)&playerChar, sizeof(SCHARACTER), PROTOCOL::BRCAST_MOVE_RP);
		//room->GetClientSession(0)->Send();
		//room->GetClientSession(1)->Send();

		break;
	}

	case PROTOCOL::INITCOMPLETE_RQ:
	{
		SINITCOMPLETE sInit;
		memset(&sInit, 0, sizeof(SINITCOMPLETE));
		memcpy(&sInit, pack.mBuffer + sizeof(SHEAD), sizeof(SINITCOMPLETE));

		InGameRoom* room = SearchRoom(pack.mSession->GetRoomNum());

		room->SetInitComplete(sInit.mPlayerIndex, sInit.mComplete);

		if (room->GetInitComplete(0) == true && room->GetInitComplete(1) == true)
		{
			room->StartGameLogicThread();
		}

		break;
	}

	}
}