#pragma once
#include "ClientSession.h"
#include "MYSQLManager.h"
#include "Crypt.h"


// GameDBManager
// 게임에서 DB 관리를 위해 사용하는 클래스
// MYSQLManager 을 상속받아 이 게임에 필요한 DB 접근방법을 새로이 정의함

class GameDBManager : public MYSQLManager
{
public:
	GameDBManager();
	~GameDBManager() {}

	bool	Login(ClientSession* client, char* id, char* password);
	bool	Logout(ClientSession* client);
	
	bool	IncreaseWin(ClientSession* client);
	bool	IncreaseLose(ClientSession* client);

private:

};

//전역으로 사용하기 위해 extern 변수로 정의.
extern GameDBManager* g_pGameDBManager;