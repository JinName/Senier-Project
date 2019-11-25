#pragma once

#include "stdafx.h"
#include <list>
#include "ClientSession.h"

/*
플레이어가 1:1 매치를 기다리는 매칭서버
매치를 누른 순서대로 List 에 push.
매칭 대기를 취소할 수 있기때문에 Queue 보단 List 를 선택한다.
*/
class MatchManager
{
public:
	
	// push back to list
	bool Push_Back(ClientSession* client);

	// 매칭을 담당하는 Thread 내에서 돌아감
	// Match List 내에 2명이상 대기중일 경우 매칭완료 후 게임 시작
	void ProcessMatchList();

private:
	std::list<ClientSession*> mMatchWaitList;
};

