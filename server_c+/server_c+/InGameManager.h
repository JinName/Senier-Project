#pragma once
#include "stdafx.h"
#include <list>

#include "ClientSession.h"

typedef struct sInGameRoom
{
	ClientSession* mPlayer1;
	ClientSession* mPlayer2;
	int mRoomNum;
}SINGAMEROOM;

/*
현재 대전 중인 플레이어를 짝지어 리스트로 저장
따로 쓰레드를 통한 프로세싱은 필요하지 않으며
대전 중인 ClientSession 정보를 관리하며
Send() / Recv() 시에 상대방에게 정보를 보내기위해 주로 사용한다.
*/
class InGameManager
{
public:
	bool InGame(ClientSession* player1, ClientSession* player2);
	bool OutGame(int roomNum);

private:
	std::list<SINGAMEROOM> mInGameRoomList;
};

