#pragma once
#include "ClientSession.h"
#include "MYSQLManager.h"
#include "Crypt.h"


// GameDBManager
// ���ӿ��� DB ������ ���� ����ϴ� Ŭ����
// MYSQLManager �� ��ӹ޾� �� ���ӿ� �ʿ��� DB ���ٹ���� ������ ������

class GameDBManager : public MYSQLManager
{
public:
	GameDBManager();
	~GameDBManager() {}

	bool	Login(ClientSession* _client, char* _id, char* _pw);
	bool	Logout(ClientSession* _client);

private:

};

//�������� ����ϱ� ���� extern ������ ����.
extern GameDBManager* g_pGameDBManager;