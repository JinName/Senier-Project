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
���� ���� ���� �÷��̾ ¦���� ����Ʈ�� ����
���� �����带 ���� ���μ����� �ʿ����� ������
���� ���� ClientSession ������ �����ϸ�
Send() / Recv() �ÿ� ���濡�� ������ ���������� �ַ� ����Ѵ�.
*/
class InGameManager
{
public:
	bool InGame(ClientSession* player1, ClientSession* player2);
	bool OutGame(int roomNum);

private:
	std::list<SINGAMEROOM> mInGameRoomList;
};

