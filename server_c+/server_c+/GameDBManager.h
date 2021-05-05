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

	bool	Login(ClientSession* client, char* id, char* password);
	bool	Logout(ClientSession* client);
	
	bool	IncreaseWin(ClientSession* client);
	bool	IncreaseLose(ClientSession* client);

private:

};

//�������� ����ϱ� ���� extern ������ ����.
extern GameDBManager* g_pGameDBManager;