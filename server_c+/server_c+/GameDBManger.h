#pragma once
#include "MYSQLManager.h"

// GameDBManger
// 게임에서 DB 관리를 위해 사용하는 클래스
// MYSQLManager 을 상속받아 이 게임에 필요한 DB 접근방법을 새로이 정의함

class GameDBManger : public MYSQLManager
{
public:
	GameDBManger() {}
	~GameDBManger() {}

	bool	Login(const char* _id, const char* _pw);
};

//전역으로 사용하기 위해 extern 변수로 정의.
extern GameDBManger* g_pGameDBManager;