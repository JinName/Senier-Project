#pragma once
#include "MYSQLManager.h"
#include "Crypt.h"

// GameDBManger
// ���ӿ��� DB ������ ���� ����ϴ� Ŭ����
// MYSQLManager �� ��ӹ޾� �� ���ӿ� �ʿ��� DB ���ٹ���� ������ ������

class GameDBManger : public MYSQLManager
{
public:
	GameDBManger() {}
	~GameDBManger() {}

	bool	Login(char* _id, char* _pw);
};

//�������� ����ϱ� ���� extern ������ ����.
extern GameDBManger* g_pGameDBManager;